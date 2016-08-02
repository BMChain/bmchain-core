
#pragma once

#include <fc/api.hpp>

namespace steemit { namespace app {
   struct api_context;
} }

namespace steemit { namespace plugin { namespace promoted_posts {

namespace detail {
class promoted_posts_api_impl;
}

class promoted_posts_api
{
   public:
      promoted_posts_api( const steemit::app::api_context& ctx );

      void on_api_startup();

      // TODO:  Add API methods here

   private:
      std::shared_ptr< detail::promoted_posts_api_impl > my;
};

} } }

FC_API( steemit::plugin::promoted_posts::promoted_posts_api,
   // TODO:  Add method bubble list here
   )
