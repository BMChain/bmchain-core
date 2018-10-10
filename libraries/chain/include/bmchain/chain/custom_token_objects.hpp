#pragma once

//#include <bmchain/protocol/authority.hpp>
//#include <bmchain/protocol/bmchain_operations.hpp>
//
//#include <bmchain/chain/bmchain_object_types.hpp>
//#include <bmchain/chain/comment_object.hpp>
//
//#include <boost/multi_index/composite_key.hpp>
//#include <boost/multiprecision/cpp_int.hpp>

#include <bmchain/chain/bmchain_objects.hpp>

namespace bmchain { namespace chain {

   enum class smt_phase : uint8_t
   {
      account_elevated,
      setup_completed,
      contribution_begin_time_completed,
      contribution_end_time_completed,
      launch_time_completed,              /// launch window opened
      launch_failed,                      /// launch window closed with either not enough contributions or some cap not revealed
      launch_success                      /// enough contributions were declared and caps revealed before launch windows closed
   };

   class custom_token_object : public object< custom_token_object_type, custom_token_object > {
   public:
      custom_token_object() = delete;

      template<typename Constructor, typename Allocator>
      custom_token_object(Constructor &&c, allocator<Allocator> a) {
         c(*this);
      }

      id_type id;
      account_name_type control_account;
      asset_symbol_type symbol;
      asset             current_supply;
      asset             reward_fund;
      uint16_t          inflation_rate = 0;
      time_point_sec    generation_time;

      time_point_sec       schedule_time =  BMCHAIN_GENESIS_TIME;;
      bmchain::protocol::custom_token_emissions_unit emissions_unit;
      uint32_t             interval_seconds = 0;
      uint32_t             interval_count = 0;
   };

   class account_balance_object : public object< account_balance_object_type, account_balance_object > {
      account_balance_object() = delete;

   public:
      template<typename Constructor, typename Allocator>
      account_balance_object(Constructor &&c, allocator <Allocator> a) {
         c(*this);
      }

      id_type id;
      /// Name of the account, the balance is held for.
      asset_symbol_type symbol;
      account_name_type owner;
      asset             balance;
   };

   class custom_token_inflation_event_object : public object< custom_token_inflation_event_object_type, custom_token_inflation_event_object >
   {
      custom_token_inflation_event_object() = delete;

   public:
      template<typename Constructor, typename Allocator>
      custom_token_inflation_event_object(Constructor &&c, allocator <Allocator> a) {
         c(*this);
      }

      // id_type is actually oid<smt_event_token_object>
      id_type id;

      custom_token_inflation_event_id_type parent;

      smt_phase phase = smt_phase::setup_completed;

      time_point_sec generation_begin_time;
      time_point_sec generation_end_time;
      time_point_sec announced_launch_time;
      time_point_sec launch_expiration_time;
   };

   struct by_symbol;
   struct by_control_account;
   typedef multi_index_container<
              custom_token_object,
              indexed_by<
                      ordered_unique< tag< by_id >, member< custom_token_object, custom_token_id_type, &custom_token_object::id > >,
                      ordered_unique< tag< by_control_account >,
                              composite_key< custom_token_object,
                                      member< custom_token_object, account_name_type,  &custom_token_object::control_account >,
                                      member< custom_token_object, custom_token_id_type,  &custom_token_object::id >
                              >,
                              composite_key_compare< std::less< account_name_type >, std::greater< custom_token_id_type > >
                      >,
                      ordered_unique< tag< by_symbol >,
                              composite_key< custom_token_object,
                                      member< custom_token_object, asset_symbol_type, &custom_token_object::symbol >
                              >
                      >,
                      ordered_unique< tag< by_schedule_time >,
                              composite_key< custom_token_object,
                                      member< custom_token_object, time_point_sec,  &custom_token_object::schedule_time >,
                                      member< custom_token_object, custom_token_id_type,  &custom_token_object::id >
                              >,
                              composite_key_compare< std::greater< time_point_sec >, std::greater< custom_token_id_type > >
                      >
              >,
              allocator< custom_token_object >
   > custom_token_index;

   typedef multi_index_container<
              account_balance_object,
              indexed_by<
                      ordered_unique< tag< by_id >, member< account_balance_object, account_balance_id_type, &account_balance_object::id > >,
                      ordered_unique< tag< by_symbol >,
                              composite_key< account_balance_object,
                                      member< account_balance_object, asset_symbol_type,  &account_balance_object::symbol >,
                                      member< account_balance_object, account_balance_id_type, &account_balance_object::id >
                              >,
                              composite_key_compare< std::less< asset_symbol_type >, std::greater< account_balance_id_type > >
                      >,
                      ordered_unique< tag< by_owner >,
                              composite_key< account_balance_object,
                                      member< account_balance_object, account_name_type,  &account_balance_object::owner >,
                                      member< account_balance_object, account_balance_id_type,  &account_balance_object::id >
                              >,
                              composite_key_compare< std::less< account_name_type >, std::greater< account_balance_id_type > >
                      >
              >,
              allocator< account_balance_object >
   > account_balance_index;

}}

FC_REFLECT( bmchain::chain::custom_token_object,
                    (id)(control_account)(symbol)(current_supply)(inflation_rate)(reward_fund)(generation_time)(schedule_time)
                    (emissions_unit)(interval_seconds)(interval_count) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::custom_token_object, bmchain::chain::custom_token_index )

FC_REFLECT( bmchain::chain::account_balance_object,(id)(owner)(balance) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::account_balance_object, bmchain::chain::account_balance_index )