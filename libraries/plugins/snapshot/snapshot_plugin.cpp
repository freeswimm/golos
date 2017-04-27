#include <steemit/snapshot/snapshot_plugin.hpp>

namespace steemit {
    namespace plugin {
        namespace snapshot {

            snapshot_plugin::snapshot_plugin(steemit::application::application *app)
                    : steemit::application::plugin(app) {
            }

            snapshot_plugin::~snapshot_plugin() {
            }

            std::string snapshot_plugin::plugin_name() const {
                return "snapshot";
            }

            void snapshot_plugin::plugin_initialize(const boost::program_options::variables_map &options) {
                _message = "hello world";
            }

            void snapshot_plugin::plugin_startup() {
                app().register_api_factory<snapshot_api>("snapshot_api");
            }

            std::string snapshot_plugin::get_message() {
                std::stringstream result;
                result << _message << " -- users have viewed this message "
                       << (_plugin_call_count++) << " times";
                return result.str();
            }

            snapshot_api::snapshot_api(const steemit::application::api_context &ctx)
                    : _app(ctx.app) {
            }

            void snapshot_api::on_api_startup() {
            }

            std::string snapshot_api::get_message() {
                std::stringstream result;
                std::shared_ptr<snapshot_plugin> plug = _app.get_plugin<snapshot_plugin>("hello_api");
                result << plug->get_message()
                       << " -- you've viewed this message "
                       << (_api_call_count++) << " times";
                return result.str();
            }
        }
    }
}

/**
 * The STEEMIT_DEFINE_PLUGIN() macro will define a steemit::plugin::create_hello_api_plugin()
 * factory method which is expected by the manifest.
 */

STEEMIT_DEFINE_PLUGIN(snapshot, steemit::plugin::snapshot::snapshot_plugin)