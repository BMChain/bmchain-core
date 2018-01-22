
#pragma once

#include <memory>
#include <string>
#include <vector>

namespace bmchain { namespace app {

class abstract_plugin;
class application;

} }

namespace bmchain { namespace plugin {

void initialize_plugin_factories();
std::shared_ptr< bmchain::app::abstract_plugin > create_plugin( const std::string& name, bmchain::app::application* app );
std::vector< std::string > get_available_plugins();

} }
