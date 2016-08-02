
#include <steemit/app/api_context.hpp>
#include <steemit/app/application.hpp>

#include <steemit/plugins/promoted_posts/promoted_posts_api.hpp>
#include <steemit/plugins/promoted_posts/promoted_posts_plugin.hpp>

namespace steemit { namespace plugin { namespace promoted_posts {

namespace detail {

class promoted_posts_api_impl
{
   public:
      promoted_posts_api_impl( steemit::app::application& _app );

      std::shared_ptr< steemit::plugin::promoted_posts::promoted_posts_plugin > get_plugin();

      steemit::app::application& app;
};

promoted_posts_api_impl::promoted_posts_api_impl( steemit::app::application& _app ) : app( _app )
{}

std::shared_ptr< steemit::plugin::promoted_posts::promoted_posts_plugin > promoted_posts_api_impl::get_plugin()
{
   return app.get_plugin< promoted_posts_plugin >( "promoted_posts" );
}

} // detail

promoted_posts_api::promoted_posts_api( const steemit::app::api_context& ctx )
{
   my = std::make_shared< detail::promoted_posts_api_impl >(ctx.app);
}

void promoted_posts_api::on_api_startup() { }

} } } // steemit::plugin::promoted_posts
