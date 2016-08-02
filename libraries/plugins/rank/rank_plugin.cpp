

#include <steemit/plugins/rank/rank_api.hpp>
#include <steemit/plugins/rank/rank_plugin.hpp>

#include <string>

namespace steemit { namespace plugin { namespace rank {

rank_plugin::rank_plugin() {}
rank_plugin::~rank_plugin() {}

std::string rank_plugin::plugin_name()const
{
   return "rank";
}

void rank_plugin::plugin_initialize( const boost::program_options::variables_map& options )
{
}

void rank_plugin::plugin_startup()
{
   chain::database& db = database();

   app().register_api_factory< rank_api >( "rank_api" );
}

void rank_plugin::plugin_shutdown()
{
}

void rank_plugin::register_ranking( const std::string& name, std::shared_ptr< ranking > r )
{
   _ranking_map.emplace( name, r );
   return;
}

} } } // steemit::plugin::rank

STEEMIT_DEFINE_PLUGIN( rank, steemit::plugin::rank::rank_plugin )
