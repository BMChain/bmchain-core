#pragma once

#include <bmchain/protocol/authority.hpp>
#include <bmchain/protocol/bmchain_operations.hpp>

#include <bmchain/chain/bmchain_object_types.hpp>
#include <bmchain/chain/comment_object.hpp>

#include <boost/multi_index/composite_key.hpp>
#include <boost/multiprecision/cpp_int.hpp>


namespace bmchain { namespace chain {

//   class custom_token_object : public object< custom_token_object_type, custom_token_object >
//   {
//      custom_token_object() = delete;
//
//   public:
//      template< typename Constructor, typename Allocator >
//      custom_token_object( Constructor&& c, allocator< Allocator > a )
//      :symbol( a )
//      {
//         c( *this );
//      }
//
//      id_type           id;
//      account_name_type control_account;
//      shared_string     symbol;
//      share_type        current_supply = 0;
//      time_point_sec    generation_time;
//   };
//
//   struct by_id;
//   struct by_symbol;
//   struct by_control_account;
//   typedef multi_index_container <
//      custom_token_object,
//      indexed_by <
//              ordered_unique< tag< by_symbol >, member< custom_token_object, custom_token_id_type, &custom_token_object::id > >
//         >,
//         allocator< custom_token_object >
//      > custom_token_index;
//         //ordered_unique< tag< by_symbol >, member< custom_token_object, shared_string, &custom_token_object::symbol >, strcmp_less >,
//    //     ordered_unique< tag< by_control_account >, member< custom_token_object, account_name_type, &custom_token_object::control_account > >
//   //       ordered_unique< tag< by_id >,
//   //           member< custom_token_object, custom_token_id_type, &custom_token_object::id > >,
////            ordered_unique< tag< by_symbol >,
////            composite_key< custom_token_object,
////               member< custom_token_object, time_point_sec, &custom_token_object::generation_time >
////            >,
////            composite_key_compare< std::less< time_point_sec > >
////         >
////         ordered_non_unique< tag< by_control_account >,
////            member< custom_token_object, account_name_type, &custom_token_object::control_account > >
//

}}

//FC_REFLECT( bmchain::chain::custom_token_object, (id)(control_account)(symbol)(current_supply)(generation_time))
//
//CHAINBASE_SET_INDEX_TYPE( bmchain::chain::custom_token_object, bmchain::chain::custom_token_index )

