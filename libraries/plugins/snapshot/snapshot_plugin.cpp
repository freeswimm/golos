#include <steemit/snapshot/snapshot_plugin.hpp>
#include <steemit/snapshot/snapshot_state.hpp>

#include <steemit/chain/index.hpp>
#include <steemit/chain/account_object.hpp>
#include <steemit/chain/steem_object_types.hpp>

#include <boost/multi_index/composite_key.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

namespace steemit {
    namespace plugin {
        namespace snapshot {
            namespace detail {
#ifndef ACCOUNT_BY_KEY_SPACE_ID
#define ACCOUNT_BY_KEY_SPACE_ID 11
#endif

                enum account_by_key_object_types {
                    key_lookup_object_type = (ACCOUNT_BY_KEY_SPACE_ID << 8)
                };

                class key_lookup_object
                        : public chainbase::object<key_lookup_object_type, key_lookup_object> {
                public:
                    template<typename Constructor, typename Allocator>
                    key_lookup_object(Constructor &&c, chainbase::allocator<Allocator> a) {
                        c(*this);
                    }

                    id_type id;

                    chain::public_key_type key;
                    chain::account_name_type account;
                };

                typedef key_lookup_object::id_type key_lookup_id_type;

                using namespace boost::multi_index;

                struct by_key;

                typedef multi_index_container<
                        key_lookup_object,
                        indexed_by<
                                ordered_unique<tag<chain::by_id>, member<key_lookup_object, key_lookup_id_type, &key_lookup_object::id>>,
                                ordered_unique<tag<by_key>,
                                        composite_key<key_lookup_object,
                                                member<key_lookup_object, chain::public_key_type, &key_lookup_object::key>,
                                                member<key_lookup_object, chain::account_name_type, &key_lookup_object::account>
                                        >
                                >
                        >,
                        chainbase::allocator<key_lookup_object>
                > key_lookup_index;

                class snapshot_plugin_impl {
                public:
                    snapshot_plugin_impl(snapshot_plugin &plugin)
                            : self(plugin) {

                    }

                    steemit::chain::database &database() {
                        return self.database();
                    }

                    void clear_cache();

                    void cache_auths(const chain::account_authority_object &a);

                    void update_key_lookup(const chain::account_authority_object &a);

                    flat_set<chain::public_key_type> cached_keys;
                    snapshot_plugin &self;
                };
            }

            void detail::snapshot_plugin_impl::clear_cache() {
                cached_keys.clear();
            }

            void detail::snapshot_plugin_impl::cache_auths(const chain::account_authority_object &a) {
                for (const auto &item : a.owner.key_auths) {
                    cached_keys.insert(item.first);
                }
                for (const auto &item : a.active.key_auths) {
                    cached_keys.insert(item.first);
                }
                for (const auto &item : a.posting.key_auths) {
                    cached_keys.insert(item.first);
                }
            }

            void detail::snapshot_plugin_impl::update_key_lookup(const chain::account_authority_object &a) {
                chain::database &db = database();
                flat_set<chain::public_key_type> new_keys;

                // Construct the set of keys in the account's authority
                for (const auto &item : a.owner.key_auths) {
                    new_keys.insert(item.first);
                }
                for (const auto &item : a.active.key_auths) {
                    new_keys.insert(item.first);
                }
                for (const auto &item : a.posting.key_auths) {
                    new_keys.insert(item.first);
                }

                // For each key that needs a lookup
                for (const auto &key : new_keys) {
                    // If the key was not in the authority, add it to the lookup
                    if (cached_keys.find(key) == cached_keys.end()) {
                        auto lookup_itr = db.find<key_lookup_object, by_key>(std::make_tuple(key, a.account));

                        if (lookup_itr == nullptr) {
                            db.create<key_lookup_object>([&](key_lookup_object &o) {
                                o.key = key;
                                o.account = a.account;
                            });
                        }
                    } else {
                        // If the key was already in the auths, remove it from the set so we don't delete it
                        cached_keys.erase(key);
                    }
                }

                // Loop over the keys that were in authority but are no longer and remove them from the lookup
                for (const auto &key : cached_keys) {
                    auto lookup_itr = db.find<key_lookup_object, by_key>(std::make_tuple(key, a.account));

                    if (lookup_itr != nullptr) {
                        db.remove(*lookup_itr);
                    }
                }

                cached_keys.clear();
            }

            snapshot_plugin::snapshot_plugin(steemit::application::application *app)
                    : plugin(app),
                      my(new detail::snapshot_plugin_impl(*this)) {
            }

            snapshot_plugin::~snapshot_plugin() {
            }

            std::string snapshot_plugin::plugin_name() const {
                return SNAPSHOT_PLUGIN_NAME;
            }

            void snapshot_plugin::plugin_initialize(const boost::program_options::variables_map &options) {
                try {
                    ilog("Initializing snapshot plugin");
                    chain::database &db = database();

                    chain::add_plugin_index<detail::key_lookup_index>(db);
                }
                FC_CAPTURE_AND_RETHROW()

                this->options = options;
                std::vector<std::string> default_snapshots = {
                        "snapshot5392323.json"
                };

                STEEMIT_LOAD_VALUE_SET(options, "snapshot-file", default_snapshots, string);

                if (this->options.count("snapshot-file")) {
                    chain::database &db = database();

                    const vector<string> snapshots = this->options["snapshot-file"].as<vector<string>>();

                    for (const vector<string>::value_type &iterator : snapshots) {
                        FC_ASSERT(fc::exists(iterator), "Snapshot file '${file}' was not found.", ("file", iterator));

                        snapshot_state snapshot = fc::json::from_file(fc::path(iterator)).as<snapshot_state>();
                        for (account_summary &account : snapshot.accounts) {
                            db.create<chain::account_object>([&](chain::account_object &a) {
                                a.name = account.name;
                                a.memo_key = account.keys.memo_key;
                                a.json_metadata = "{created_at: 'GENESIS'}";
                                a.recovery_account = STEEMIT_INIT_MINER_NAME;
                            });

                            my->update_key_lookup(db.create<chain::account_authority_object>([&](chain::account_authority_object &auth) {
                                auth.account = account.name;
                                auth.owner.weight_threshold = 1;
                                auth.owner = account.keys.owner_key;
                                auth.active = account.keys.active_key;
                                auth.posting = account.keys.posting_key;
                            }));
                        }

                        loaded_snapshots.insert({iterator,
                                                 fc::sha256(boost::iostreams::mapped_file_source(fc::path(iterator).string()).data())
                        });
                    }
                }
            }

            void snapshot_plugin::plugin_set_program_options(boost::program_options::options_description &command_line_options, boost::program_options::options_description &config_file_options) {
                command_line_options.add_options()
                        ("snapshot-file,s", boost::program_options::value<string>()->composing()->multitoken(), "Snapshot files to load");
                config_file_options.
                        add(command_line_options);
            }

            void snapshot_plugin::plugin_startup() {

            }

            const unordered_map<string, string> &snapshot_plugin::get_loaded_snapshots() const {
                return loaded_snapshots;
            }
        }
    }
}

/**
 * The STEEMIT_DEFINE_PLUGIN() macro will define a steemit::plugin::create_hello_api_plugin()
 * factory method which is expected by the manifest.
 */

FC_REFLECT(steemit::plugin::snapshot::detail::key_lookup_object, (id)(key)(account))
CHAINBASE_SET_INDEX_TYPE(steemit::plugin::snapshot::detail::key_lookup_object, steemit::plugin::snapshot::detail::key_lookup_index)

STEEMIT_DEFINE_PLUGIN(snapshot, steemit::plugin::snapshot::snapshot_plugin)