
#pragma once

#include <steemit/plugins/rank/ranking.hpp>

#include <fc/api.hpp>

namespace steemit { namespace app {
   struct api_context;
} }

namespace steemit { namespace plugin { namespace rank {

namespace detail {
class rank_api_impl;
}

struct get_by_rank_args : public query
{
   std::string ranking;
};

struct get_by_rank_result
{
   std::vector< fc::variant > objects;
};

struct get_available_rankings_args
{
};

struct get_available_rankings_result
{
   std::vector< std::string > rankings;
};

class rank_api
{
   public:
      rank_api( const steemit::app::api_context& ctx );

      void on_api_startup();

      get_available_rankings_result get_available_rankings( get_available_rankings_args args );
      get_by_rank_result get_by_rank( get_by_rank_args args );

   private:
      std::shared_ptr< detail::rank_api_impl > my;
};

} } }

FC_REFLECT_DERIVED( steemit::plugin::rank::get_by_rank_args, (steemit::plugin::rank::query), (ranking) )
FC_REFLECT( steemit::plugin::rank::get_by_rank_result, (objects) )
FC_REFLECT( steemit::plugin::rank::get_available_rankings_args, )
FC_REFLECT( steemit::plugin::rank::get_available_rankings_result, (rankings) )

FC_API( steemit::plugin::rank::rank_api,
   (get_available_rankings)
   (get_by_rank)
   )
