#pragma once

#include <bmchain/protocol/authority.hpp>
#include <bmchain/protocol/bmchain_operations.hpp>

#include <bmchain/chain/bmchain_object_types.hpp>
#include <bmchain/chain/comment_object.hpp>

#include <boost/multi_index/composite_key.hpp>
#include <boost/multiprecision/cpp_int.hpp>


namespace bmchain { namespace chain {

   using bmchain::protocol::asset;
   using bmchain::protocol::price;
   using bmchain::protocol::asset_symbol_type;

   typedef protocol::fixed_string_16 reward_fund_name_type;

   /**
    *  This object is used to track pending requests to convert
    */
   class convert_request_object : public object< convert_request_object_type, convert_request_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         convert_request_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         convert_request_object(){}

         id_type           id;

         account_name_type owner;
         uint32_t          requestid = 0; ///< id set by owner, the owner,requestid pair must be unique
         asset             amount;
         time_point_sec    conversion_date; ///< at this time the feed_history_median_price * amount
   };


   class escrow_object : public object< escrow_object_type, escrow_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         escrow_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         escrow_object(){}

         id_type           id;

         uint32_t          escrow_id = 20;
         account_name_type from;
         account_name_type to;
         account_name_type agent;
         time_point_sec    ratification_deadline;
         time_point_sec    escrow_expiration;
         asset             bmt_balance;
         asset             pending_fee;
         bool              to_approved = false;
         bool              agent_approved = false;
         bool              disputed = false;

         bool              is_approved()const { return to_approved && agent_approved; }
   };


   class savings_withdraw_object : public object< savings_withdraw_object_type, savings_withdraw_object >
   {
      savings_withdraw_object() = delete;

      public:
         template< typename Constructor, typename Allocator >
         savings_withdraw_object( Constructor&& c, allocator< Allocator > a )
            :memo( a )
         {
            c( *this );
         }

         id_type           id;

         account_name_type from;
         account_name_type to;
         shared_string     memo;
         uint32_t          request_id = 0;
         asset             amount;
         time_point_sec    complete;
   };

   /**
    *  This object gets updated once per hour, on the hour
    */
   class feed_history_object  : public object< feed_history_object_type, feed_history_object >
   {
      feed_history_object() = delete;

      public:
         template< typename Constructor, typename Allocator >
         feed_history_object( Constructor&& c, allocator< Allocator > a )
            :price_history( a.get_segment_manager() )
         {
            c( *this );
         }

         id_type                                   id;

         price                                     current_median_history; ///< the current median of the price history, used as the base for convert operations
         bip::deque< price, allocator< price > >   price_history; ///< tracks this last week of median_feed one per hour
   };


   /**
    *  @brief an offer to sell a amount of a asset at a specified exchange rate by a certain time
    *  @ingroup object
    *  @ingroup protocol
    *  @ingroup market
    *
    *  This limit_order_objects are indexed by @ref expiration and is automatically deleted on the first block after expiration.
    */
   class limit_order_object : public object< limit_order_object_type, limit_order_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         limit_order_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         limit_order_object(){}

         id_type           id;

         time_point_sec    created;
         time_point_sec    expiration;
         account_name_type seller;
         uint32_t          orderid = 0;
         share_type        for_sale; ///< asset id is sell_price.base.symbol
         price             sell_price;

         pair< asset_symbol_type, asset_symbol_type > get_market()const
         {
            return sell_price.base.symbol < sell_price.quote.symbol ?
                std::make_pair( sell_price.base.symbol, sell_price.quote.symbol ) :
                std::make_pair( sell_price.quote.symbol, sell_price.base.symbol );
         }

         asset amount_for_sale()const   { return asset( for_sale, sell_price.base.symbol ); }
         asset amount_to_receive()const { return amount_for_sale() * sell_price; }
   };


   /**
    * @breif a route to send withdrawn vesting shares.
    */
   class withdraw_rep_route_object : public object< withdraw_rep_route_object_type, withdraw_rep_route_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         withdraw_rep_route_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         withdraw_rep_route_object(){}

         id_type  id;

         account_id_type   from_account;
         account_id_type   to_account;
         uint16_t          percent = 0;
         bool              auto_vest = false;
   };


   class decline_voting_rights_request_object : public object< decline_voting_rights_request_object_type, decline_voting_rights_request_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         decline_voting_rights_request_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         decline_voting_rights_request_object(){}

         id_type           id;

         account_id_type   account;
         time_point_sec    effective_date;
   };

   enum curve_id
   {
      quadratic,
      quadratic_curation,
      linear,
      square_root,
      power17
   };

   class reward_fund_object : public object< reward_fund_object_type, reward_fund_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         reward_fund_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         reward_fund_object() {}

         reward_fund_id_type     id;
         reward_fund_name_type   name;
         asset                   reward_balance = asset( 0, BMT_SYMBOL );
         fc::uint128_t           recent_claims = 0;
         time_point_sec          last_update;
         uint128_t               content_constant = 0;
         uint16_t                percent_curation_rewards = 0;
         uint16_t                percent_content_rewards = 0;
         curve_id                author_reward_curve = power17; // quadratic; // linear;
         curve_id                curation_reward_curve = square_root;
   };


   class content_order_object : public object< content_order_object_type, content_order_object >
   {
      public:
         content_order_object() = delete;

         template< typename Constructor, typename Allocator >
         content_order_object( Constructor&& c, allocator< Allocator > a ) : permlink(a), json_metadata(a)
         {
            c( *this );
         }

         enum order_status{open, completed, canceled};

         id_type           id;

         time_point_sec    sent_time;
         account_name_type author;
         shared_string     permlink;
         account_name_type owner;
         asset             price = asset( 0, BMT_SYMBOL );
         order_status      status = order_status::open;
         shared_string     json_metadata;
   };

#ifndef STEEM_SMT_TEST_SPACE_ID
#define STEEM_SMT_TEST_SPACE_ID 13
#endif

   enum smt_test_object_types {
      smt_token_object_type = (STEEM_SMT_TEST_SPACE_ID << 8)
   };

   class smt_token_object : public object<smt_token_object_type, smt_token_object> {
      public:
         template<typename Constructor, typename Allocator>
         smt_token_object(Constructor &&c, allocator<Allocator> a) {
            c(*this);
         }

         id_type id;

         account_name_type control_account;
         uint8_t decimal_places = 0;
         int64_t max_supply = BMCHAIN_MAX_SHARE_SUPPLY;

         time_point_sec generation_begin_time;
         time_point_sec generation_end_time;
         time_point_sec announced_launch_time;
         time_point_sec launch_expiration_time;
   };

   typedef smt_token_object::id_type smt_token_id_type;

   struct by_price;
   struct by_expiration;
   struct by_account;
   typedef multi_index_container<
      limit_order_object,
      indexed_by<
         ordered_unique< tag< by_id >, member< limit_order_object, limit_order_id_type, &limit_order_object::id > >,
         ordered_non_unique< tag< by_expiration >, member< limit_order_object, time_point_sec, &limit_order_object::expiration > >,
         ordered_unique< tag< by_price >,
            composite_key< limit_order_object,
               member< limit_order_object, price, &limit_order_object::sell_price >,
               member< limit_order_object, limit_order_id_type, &limit_order_object::id >
            >,
            composite_key_compare< std::greater< price >, std::less< limit_order_id_type > >
         >,
         ordered_unique< tag< by_account >,
            composite_key< limit_order_object,
               member< limit_order_object, account_name_type, &limit_order_object::seller >,
               member< limit_order_object, uint32_t, &limit_order_object::orderid >
            >
         >
      >,
      allocator< limit_order_object >
   > limit_order_index;

   struct by_owner;
   struct by_conversion_date;
   typedef multi_index_container<
      convert_request_object,
      indexed_by<
         ordered_unique< tag< by_id >, member< convert_request_object, convert_request_id_type, &convert_request_object::id > >,
         ordered_unique< tag< by_conversion_date >,
            composite_key< convert_request_object,
               member< convert_request_object, time_point_sec, &convert_request_object::conversion_date >,
               member< convert_request_object, convert_request_id_type, &convert_request_object::id >
            >
         >,
         ordered_unique< tag< by_owner >,
            composite_key< convert_request_object,
               member< convert_request_object, account_name_type, &convert_request_object::owner >,
               member< convert_request_object, uint32_t, &convert_request_object::requestid >
            >
         >
      >,
      allocator< convert_request_object >
   > convert_request_index;

   struct by_owner;
   struct by_volume_weight;

   typedef multi_index_container<
      feed_history_object,
      indexed_by<
         ordered_unique< tag< by_id >, member< feed_history_object, feed_history_id_type, &feed_history_object::id > >
      >,
      allocator< feed_history_object >
   > feed_history_index;

   struct by_withdraw_route;
   struct by_destination;
   typedef multi_index_container<
      withdraw_rep_route_object,
      indexed_by<
         ordered_unique< tag< by_id >, member< withdraw_rep_route_object, withdraw_rep_route_id_type, &withdraw_rep_route_object::id > >,
         ordered_unique< tag< by_withdraw_route >,
            composite_key< withdraw_rep_route_object,
               member< withdraw_rep_route_object, account_id_type, &withdraw_rep_route_object::from_account >,
               member< withdraw_rep_route_object, account_id_type, &withdraw_rep_route_object::to_account >
            >,
            composite_key_compare< std::less< account_id_type >, std::less< account_id_type > >
         >,
         ordered_unique< tag< by_destination >,
            composite_key< withdraw_rep_route_object,
               member< withdraw_rep_route_object, account_id_type, &withdraw_rep_route_object::to_account >,
               member< withdraw_rep_route_object, withdraw_rep_route_id_type, &withdraw_rep_route_object::id >
            >
         >
      >,
      allocator< withdraw_rep_route_object >
   > withdraw_rep_route_index;

   struct by_from_id;
   struct by_to;
   struct by_agent;
   struct by_ratification_deadline;
   typedef multi_index_container<
      escrow_object,
      indexed_by<
         ordered_unique< tag< by_id >, member< escrow_object, escrow_id_type, &escrow_object::id > >,
         ordered_unique< tag< by_from_id >,
            composite_key< escrow_object,
               member< escrow_object, account_name_type,  &escrow_object::from >,
               member< escrow_object, uint32_t, &escrow_object::escrow_id >
            >
         >,
         ordered_unique< tag< by_to >,
            composite_key< escrow_object,
               member< escrow_object, account_name_type,  &escrow_object::to >,
               member< escrow_object, escrow_id_type, &escrow_object::id >
            >
         >,
         ordered_unique< tag< by_agent >,
            composite_key< escrow_object,
               member< escrow_object, account_name_type,  &escrow_object::agent >,
               member< escrow_object, escrow_id_type, &escrow_object::id >
            >
         >,
         ordered_unique< tag< by_ratification_deadline >,
            composite_key< escrow_object,
               const_mem_fun< escrow_object, bool, &escrow_object::is_approved >,
               member< escrow_object, time_point_sec, &escrow_object::ratification_deadline >,
               member< escrow_object, escrow_id_type, &escrow_object::id >
            >,
            composite_key_compare< std::less< bool >, std::less< time_point_sec >, std::less< escrow_id_type > >
         >
      >,
      allocator< escrow_object >
   > escrow_index;

   struct by_from_rid;
   struct by_to_complete;
   struct by_complete_from_rid;
   typedef multi_index_container<
      savings_withdraw_object,
      indexed_by<
         ordered_unique< tag< by_id >, member< savings_withdraw_object, savings_withdraw_id_type, &savings_withdraw_object::id > >,
         ordered_unique< tag< by_from_rid >,
            composite_key< savings_withdraw_object,
               member< savings_withdraw_object, account_name_type,  &savings_withdraw_object::from >,
               member< savings_withdraw_object, uint32_t, &savings_withdraw_object::request_id >
            >
         >,
         ordered_unique< tag< by_to_complete >,
            composite_key< savings_withdraw_object,
               member< savings_withdraw_object, account_name_type,  &savings_withdraw_object::to >,
               member< savings_withdraw_object, time_point_sec,  &savings_withdraw_object::complete >,
               member< savings_withdraw_object, savings_withdraw_id_type, &savings_withdraw_object::id >
            >
         >,
         ordered_unique< tag< by_complete_from_rid >,
            composite_key< savings_withdraw_object,
               member< savings_withdraw_object, time_point_sec,  &savings_withdraw_object::complete >,
               member< savings_withdraw_object, account_name_type,  &savings_withdraw_object::from >,
               member< savings_withdraw_object, uint32_t, &savings_withdraw_object::request_id >
            >
         >
      >,
      allocator< savings_withdraw_object >
   > savings_withdraw_index;

   struct by_account;
   struct by_effective_date;
   typedef multi_index_container<
      decline_voting_rights_request_object,
      indexed_by<
         ordered_unique< tag< by_id >, member< decline_voting_rights_request_object, decline_voting_rights_request_id_type, &decline_voting_rights_request_object::id > >,
         ordered_unique< tag< by_account >,
            member< decline_voting_rights_request_object, account_id_type, &decline_voting_rights_request_object::account >
         >,
         ordered_unique< tag< by_effective_date >,
            composite_key< decline_voting_rights_request_object,
               member< decline_voting_rights_request_object, time_point_sec, &decline_voting_rights_request_object::effective_date >,
               member< decline_voting_rights_request_object, account_id_type, &decline_voting_rights_request_object::account >
            >,
            composite_key_compare< std::less< time_point_sec >, std::less< account_id_type > >
         >
      >,
      allocator< decline_voting_rights_request_object >
   > decline_voting_rights_request_index;

   struct by_name;
   typedef multi_index_container<
      reward_fund_object,
      indexed_by<
         ordered_unique< tag< by_id >, member< reward_fund_object, reward_fund_id_type, &reward_fund_object::id > >,
         ordered_unique< tag< by_name >, member< reward_fund_object, reward_fund_name_type, &reward_fund_object::name > >
      >,
      allocator< reward_fund_object >
   > reward_fund_index;

   struct by_sent_time;
   struct by_author;
   struct by_owner;
   struct by_permlink;
   typedef multi_index_container<
      content_order_object,
      indexed_by<
         ordered_unique< tag< by_id >, member< content_order_object, content_order_id_type, &content_order_object::id > >,
         ordered_unique< tag< by_sent_time >, member< content_order_object, time_point_sec, &content_order_object::sent_time > >,
         ordered_unique< tag< by_author >,
            composite_key< content_order_object,
               member< content_order_object, account_name_type,  &content_order_object::author >,
               member< content_order_object, content_order_id_type,  &content_order_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::greater< content_order_id_type > >
         >,
         ordered_unique< tag< by_owner >,
            composite_key< content_order_object,
               member< content_order_object, account_name_type,  &content_order_object::owner >,
               member< content_order_object, content_order_id_type,  &content_order_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::greater< content_order_id_type > >
         >,
         ordered_unique< tag< by_permlink >,
            composite_key< content_order_object,
               member< content_order_object, account_name_type,  &content_order_object::author >,
               member< content_order_object, shared_string, &content_order_object::permlink >,
               member< content_order_object, account_name_type,  &content_order_object::owner >
            >,
            composite_key_compare< std::less< account_name_type >, strcmp_less, std::less< account_name_type > >
         >
      >,
      allocator< content_order_object >
   > content_order_index;

   struct by_control_account;
   typedef multi_index_container<
      smt_token_object,
      indexed_by<
          ordered_unique< tag< by_id >, member< smt_token_object, smt_token_id_type, &smt_token_object::id > >,
          ordered_unique< tag< by_control_account >,
             composite_key< smt_token_object,
                member< smt_token_object, account_name_type, &smt_token_object::control_account >
             >
          >
      >,
      allocator< smt_token_object >
   > smt_token_index;

} } // bmchain::chain

#include <bmchain/chain/comment_object.hpp>
#include <bmchain/chain/account_object.hpp>

FC_REFLECT_ENUM( bmchain::chain::curve_id,
                  (quadratic)(quadratic_curation)(linear)(square_root)(power17))

FC_REFLECT( bmchain::chain::limit_order_object,
             (id)(created)(expiration)(seller)(orderid)(for_sale)(sell_price) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::limit_order_object, bmchain::chain::limit_order_index )

FC_REFLECT( bmchain::chain::feed_history_object,
             (id)(current_median_history)(price_history) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::feed_history_object, bmchain::chain::feed_history_index )

FC_REFLECT( bmchain::chain::convert_request_object,
             (id)(owner)(requestid)(amount)(conversion_date) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::convert_request_object, bmchain::chain::convert_request_index )

FC_REFLECT( bmchain::chain::withdraw_rep_route_object,
             (id)(from_account)(to_account)(percent)(auto_vest) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::withdraw_rep_route_object, bmchain::chain::withdraw_rep_route_index )

FC_REFLECT( bmchain::chain::savings_withdraw_object,
             (id)(from)(to)(memo)(request_id)(amount)(complete) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::savings_withdraw_object, bmchain::chain::savings_withdraw_index )

FC_REFLECT( bmchain::chain::escrow_object,
             (id)(escrow_id)(from)(to)(agent)
             (ratification_deadline)(escrow_expiration)
             (bmt_balance)(pending_fee)
             (to_approved)(agent_approved)(disputed) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::escrow_object, bmchain::chain::escrow_index )

FC_REFLECT( bmchain::chain::decline_voting_rights_request_object,
             (id)(account)(effective_date) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::decline_voting_rights_request_object, bmchain::chain::decline_voting_rights_request_index )

FC_REFLECT( bmchain::chain::reward_fund_object,
            (id)
            (name)
            (reward_balance)
            (recent_claims)
            (last_update)
            (content_constant)
            (percent_curation_rewards)
            (percent_content_rewards)
            (author_reward_curve)
            (curation_reward_curve)
         )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::reward_fund_object, bmchain::chain::reward_fund_index )

FC_REFLECT( bmchain::chain::content_order_object,
            (id)(author)(permlink)(owner)(price)(status)(json_metadata) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::content_order_object, bmchain::chain::content_order_index )

FC_REFLECT_ENUM( bmchain::chain::content_order_object::order_status,
                 (open)(completed)(canceled))

FC_REFLECT( bmchain::chain::smt_token_object,
            (id)(control_account)(decimal_places)(max_supply)(generation_begin_time)(generation_end_time)
                    (announced_launch_time)(launch_expiration_time))
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::smt_token_object, bmchain::chain::smt_token_index )