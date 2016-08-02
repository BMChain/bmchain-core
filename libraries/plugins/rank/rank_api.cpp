
#include <steemit/app/api_context.hpp>
#include <steemit/app/application.hpp>

#include <steemit/plugins/rank/rank_api.hpp>
#include <steemit/plugins/rank/rank_plugin.hpp>

namespace steemit { namespace plugin { namespace rank {

namespace detail {

class rank_api_impl
{
   public:
      rank_api_impl( steemit::app::application& _app );

      std::shared_ptr< steemit::plugin::rank::rank_plugin > get_plugin();
      void get_available_rankings( const get_available_rankings_args& args, get_available_rankings_result& result );
      void get_by_rank( const get_by_rank_args& args, get_by_rank_result& result );

      steemit::app::application& app;
};

rank_api_impl::rank_api_impl( steemit::app::application& _app ) : app( _app )
{}

std::shared_ptr< steemit::plugin::rank::rank_plugin > rank_api_impl::get_plugin()
{
   return app.get_plugin< rank_plugin >( "rank" );
}

void rank_api_impl::get_available_rankings( const get_available_rankings_args& args, get_available_rankings_result& result )
{
   std::shared_ptr< steemit::plugin::rank::rank_plugin > rplugin = get_plugin();
   for( const std::pair< std::string, std::shared_ptr< ranking > >& p : rplugin->_ranking_map )
      result.rankings.push_back( p.first );
}

void rank_api_impl::get_by_rank( const get_by_rank_args& args, get_by_rank_result& result )
{
   std::shared_ptr< steemit::plugin::rank::rank_plugin > rplugin = get_plugin();
   auto it = rplugin->_ranking_map.find( args.ranking );
   FC_ASSERT( it != rplugin->_ranking_map.end() );
   it->second->get_objects( result.objects, args );
}

} // detail

rank_api::rank_api( const steemit::app::api_context& ctx )
{
   my = std::make_shared< detail::rank_api_impl >(ctx.app);
}

void rank_api::on_api_startup() { }

get_available_rankings_result rank_api::get_available_rankings( get_available_rankings_args args )
{
   get_available_rankings_result result;
   my->get_available_rankings( args, result );
   return result;
}

get_by_rank_result rank_api::get_by_rank( get_by_rank_args args )
{
   get_by_rank_result result;
   my->get_by_rank( args, result );
   return result;
}

} } } // steemit::plugin::rank
