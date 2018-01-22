#pragma once

#include <bmchain/account_statistics/account_statistics_plugin.hpp>

#include <fc/api.hpp>

namespace bmchain{ namespace app {
   struct api_context;
} }

namespace bmchain { namespace account_statistics {

namespace detail
{
   class account_statistics_api_impl;
}

class account_statistics_api
{
   public:
      account_statistics_api( const bmchain::app::api_context& ctx );

      void on_api_startup();

   private:
      std::shared_ptr< detail::account_statistics_api_impl > _my;
};

} } // bmchain::account_statistics

FC_API( bmchain::account_statistics::account_statistics_api, )