#ifndef GOLOS_SNAPSHOT_PLUGIN_HPP
#define GOLOS_SNAPSHOT_PLUGIN_HPP

#include <steemit/application/application.hpp>
#include <steemit/application/plugin.hpp>

#include <sstream>
#include <string>

namespace steemit {
    namespace plugin {
        namespace snapshot {
            class snapshot_plugin : public steemit::application::plugin {
            public:
                /**
                 * The plugin requires a constructor which takes app.  This is called regardless of whether the plugin is loaded.
                 * The app parameter should be passed up to the superclass constructor.
                 */
                snapshot_plugin(steemit::application::application *app);

                /**
                 * Plugin is destroyed via base class pointer, so a virtual destructor must be provided.
                 */
                virtual ~snapshot_plugin();

                /**
                 * Every plugin needs a name.
                 */
                virtual std::string plugin_name() const override;

                /**
                 * Called when the plugin is enabled, but before the database has been created.
                 */
                virtual void plugin_initialize(const boost::program_options::variables_map &options) override;

                /**
                 * Called when the plugin is enabled.
                 */
                virtual void plugin_startup() override;

                std::string get_message();

            private:
                steemit::application::application *_app;
                std::string _message;
                uint32_t _plugin_call_count = 0;
            };

            class snapshot_api {
            public:
                snapshot_api(const steemit::application::api_context &ctx);

                /**
                 * Called immediately after the constructor.  If the API class uses enable_shared_from_this,
                 * shared_from_this() is available in this method, which allows signal handlers to be registered
                 * with application::connect_signal()
                 */
                void on_api_startup();

                std::string get_message();

            private:
                steemit::application::application &_app;
                uint32_t _api_call_count = 0;
            };
        }
    }
}

FC_API(steemit::plugin::snapshot::snapshot_api, (get_message))

#endif //GOLOS_SNAPSHOT_PLUGIN_HPP