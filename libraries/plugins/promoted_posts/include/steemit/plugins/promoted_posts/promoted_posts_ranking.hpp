
#include <steemit/plugins/rank/ranking.hpp>

#include <fc/reflect/reflect.hpp>
#include <fc/optional.hpp>

#include <string>

#pragma once

namespace steemit { namespace chain {
class database;
} }

namespace steemit { namespace plugin { namespace promoted_posts {

class promoted_posts_ranking : public rank::ranking
{
   public:
      promoted_posts_ranking( const chain::database& db );
      virtual void get_objects( std::vector< fc::variant >& result, const rank::query& q ) override;

      const chain::database& _db;
};

namespace query {

struct filter
{
   std::string                    tag;
};

struct start
{
   fc::optional< std::string >    author;
   fc::optional< std::string >    permlink;
};

}

} } }

FC_REFLECT( steemit::plugin::promoted_posts::query::filter, (tag) )
FC_REFLECT( steemit::plugin::promoted_posts::query::start, (author)(permlink) )
