
#pragma once

#include <graphene/db/object.hpp>

#include <steemit/chain/steem_objects.hpp>
#include <steemit/chain/protocol/types.hpp>

namespace steemit { namespace plugin { namespace promoted_posts {

#ifndef PROMOTED_POST_SPACE_ID
#define PROMOTED_POST_SPACE_ID 9
#endif

#ifndef PROMOTED_POST_TYPE_ID
#define PROMOTED_POST_TYPE_ID 1
#endif

using namespace graphene::db;

class promoted_post_object : public graphene::db::abstract_object<promoted_post_object>
{
   public:
      static const uint8_t space_id = PROMOTED_POST_SPACE_ID;
      static const uint8_t type_id  = PROMOTED_POST_TYPE_ID;

      chain::comment_id_type    comment_id;
      std::string               tag;
      chain::share_type         promotion_amount;
};

using boost::multi_index_container;
using namespace boost::multi_index;

struct by_comment_id;
struct by_tag;

typedef multi_index_container<
   promoted_post_object,
   indexed_by<
      ordered_unique< tag< by_id >, member< object, object_id_type, &object::id > >,
      ordered_unique< tag< by_comment_id >,
         composite_key< promoted_post_object,
            member< promoted_post_object, chain::comment_id_type, &promoted_post_object::comment_id >,
            member< promoted_post_object, std::string, &promoted_post_object::tag >
         >
      >,
      ordered_unique< tag< by_tag >,
         composite_key< promoted_post_object,
            member< promoted_post_object, std::string, &promoted_post_object::tag >,
            member< promoted_post_object, chain::share_type, &promoted_post_object::promotion_amount >,
            member< object, object_id_type, &object::id >
         >,
         composite_key_compare<
            std::less< std::string >,
            std::greater< chain::share_type >,
            std::less< object_id_type >
         >
      >
   > > promoted_post_multi_index_type;

typedef generic_index< promoted_post_object, promoted_post_multi_index_type >                      promoted_post_index;

typedef object_id< PROMOTED_POST_SPACE_ID, PROMOTED_POST_TYPE_ID, promoted_post_object >           promoted_post_id_type;

} } }

FC_REFLECT_DERIVED( steemit::plugin::promoted_posts::promoted_post_object, (graphene::db::object),
                    (comment_id)(promotion_amount) )
