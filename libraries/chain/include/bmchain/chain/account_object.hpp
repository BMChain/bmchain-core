#pragma once
#include <fc/fixed_string.hpp>

#include <bmchain/protocol/authority.hpp>
#include <bmchain/protocol/bmchain_operations.hpp>

#include <bmchain/chain/bmchain_object_types.hpp>
#include <bmchain/chain/witness_objects.hpp>
#include <bmchain/chain/shared_authority.hpp>

#include <boost/multi_index/composite_key.hpp>

#include <numeric>

namespace bmchain { namespace chain {

   using bmchain::protocol::authority;

   class account_object : public object< account_object_type, account_object >
   {
      account_object() = delete;

      public:
         template<typename Constructor, typename Allocator>
         account_object( Constructor&& c, allocator< Allocator > a )
            :json_metadata( a )
         {
            c(*this);
         };

         id_type           id;

         account_name_type name;
         public_key_type   memo_key;
         shared_string     json_metadata;
         account_name_type proxy;

         time_point_sec    last_account_update;

         time_point_sec    created;
         bool              mined = true;
         bool              owner_challenged = false;
         bool              active_challenged = false;
         time_point_sec    last_owner_proved = time_point_sec::min();
         time_point_sec    last_active_proved = time_point_sec::min();
         account_name_type recovery_account;
         account_name_type reset_account = BMCHAIN_NULL_ACCOUNT;
         time_point_sec    last_account_recovery;
         uint32_t          comment_count = 0;
         uint32_t          lifetime_vote_count = 0;
         uint32_t          post_count = 0;

         bool              can_vote = true;
         uint16_t          voting_power = BMCHAIN_100_PERCENT;   ///< current voting power of this account, it falls after every vote
         time_point_sec    last_vote_time; ///< used to increase the voting power of this account the longer it goes without voting.

         asset             balance = asset( 0, BMT_SYMBOL );  ///< total liquid shares held by this account
         asset             savings_balance = asset( 0, BMT_SYMBOL );  ///< total liquid shares held by this account

         uint8_t           savings_withdraw_requests = 0;
         ///@}

         asset             reward_bmt_balance = asset( 0, BMT_SYMBOL );
         asset             reward_rep_balance = asset( 0, REP_SYMBOL );
         asset             reward_rep_bmt = asset( 0, BMT_SYMBOL );

         share_type        curation_rewards = 0;
         share_type        posting_rewards = 0;

         asset             rep_shares = asset( 0, REP_SYMBOL ); ///< total vesting shares held by this account, controls its voting power
         asset             delegated_rep_shares = asset( 0, REP_SYMBOL );
         asset             received_rep_shares = asset( 0, REP_SYMBOL );

         asset             rep_withdraw_rate = asset( 0, REP_SYMBOL ); ///< at the time this is updated it can be at most rep_shares/104
         time_point_sec    next_rep_withdrawal = fc::time_point_sec::maximum(); ///< after every withdrawal this is incremented by 1 week
         share_type        withdrawn = 0; /// Track how many shares have been withdrawn
         share_type        to_withdraw = 0; /// Might be able to look this up with operation history.
         uint16_t          withdraw_routes = 0;

         fc::array<share_type, BMCHAIN_MAX_PROXY_RECURSION_DEPTH> proxied_vsf_votes;// = std::vector<share_type>( BMCHAIN_MAX_PROXY_RECURSION_DEPTH, 0 ); ///< the total VFS votes proxied to this account

         uint16_t          witnesses_voted_for = 0;

         time_point_sec    last_post;
         time_point_sec    last_root_post = fc::time_point_sec::min();
         uint32_t          post_bandwidth = 0;

         /// This function should be used only when the account votes for a witness directly
         share_type        witness_vote_weight()const {
            return std::accumulate( proxied_vsf_votes.begin(),
                                    proxied_vsf_votes.end(),
                                    rep_shares.amount );
         }
         share_type        proxied_vsf_votes_total()const {
            return std::accumulate( proxied_vsf_votes.begin(),
                                    proxied_vsf_votes.end(),
                                    share_type() );
         }

         asset effective_rep_shares()const { return rep_shares - delegated_rep_shares + received_rep_shares; }
   };

   class account_authority_object : public object< account_authority_object_type, account_authority_object >
   {
      account_authority_object() = delete;

      public:
         template< typename Constructor, typename Allocator >
         account_authority_object( Constructor&& c, allocator< Allocator > a )
            : owner( a ), active( a ), posting( a )
         {
            c( *this );
         }

         id_type           id;

         account_name_type account;

         shared_authority  owner;   ///< used for backup control, can set owner or active
         shared_authority  active;  ///< used for all monetary operations, can set active or posting
         shared_authority  posting; ///< used for voting and posting

         time_point_sec    last_owner_update;
   };

   class rep_delegation_object : public object< rep_delegation_object_type, rep_delegation_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         rep_delegation_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         rep_delegation_object() {}

         id_type           id;
         account_name_type delegator;
         account_name_type delegatee;
         asset             rep_shares;
         time_point_sec    min_delegation_time;
   };

   class rep_delegation_expiration_object : public object< rep_delegation_expiration_object_type, rep_delegation_expiration_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         rep_delegation_expiration_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         rep_delegation_expiration_object() {}

         id_type           id;
         account_name_type delegator;
         asset             rep_shares;
         time_point_sec    expiration;
   };

   class owner_authority_history_object : public object< owner_authority_history_object_type, owner_authority_history_object >
   {
      owner_authority_history_object() = delete;

      public:
         template< typename Constructor, typename Allocator >
         owner_authority_history_object( Constructor&& c, allocator< Allocator > a )
            :previous_owner_authority( shared_authority::allocator_type( a.get_segment_manager() ) )
         {
            c( *this );
         }

         id_type           id;

         account_name_type account;
         shared_authority  previous_owner_authority;
         time_point_sec    last_valid_time;
   };

   class account_recovery_request_object : public object< account_recovery_request_object_type, account_recovery_request_object >
   {
      account_recovery_request_object() = delete;

      public:
         template< typename Constructor, typename Allocator >
         account_recovery_request_object( Constructor&& c, allocator< Allocator > a )
            :new_owner_authority( shared_authority::allocator_type( a.get_segment_manager() ) )
         {
            c( *this );
         }

         id_type           id;

         account_name_type account_to_recover;
         shared_authority  new_owner_authority;
         time_point_sec    expires;
   };

   class change_recovery_account_request_object : public object< change_recovery_account_request_object_type, change_recovery_account_request_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         change_recovery_account_request_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         id_type           id;

         account_name_type account_to_recover;
         account_name_type recovery_account;
         time_point_sec    effective_on;
   };

   struct by_name;
   struct by_proxy;
   struct by_last_post;
   struct by_next_rep_withdrawal;
   struct by_bmt_balance;
   struct by_smp_balance;
   struct by_smd_balance;
   struct by_post_count;
   struct by_vote_count;
   struct by_rep_shares;

   /**
    * @ingroup object_index
    */
   typedef multi_index_container<
      account_object,
      indexed_by<
         ordered_unique< tag< by_id >,
            member< account_object, account_id_type, &account_object::id > >,
         ordered_unique< tag< by_name >,
            member< account_object, account_name_type, &account_object::name > >,
         ordered_unique< tag< by_proxy >,
            composite_key< account_object,
               member< account_object, account_name_type, &account_object::proxy >,
               member< account_object, account_id_type, &account_object::id >
            > /// composite key by proxy
         >,
         ordered_unique< tag< by_next_rep_withdrawal >,
            composite_key< account_object,
               member< account_object, time_point_sec, &account_object::next_rep_withdrawal >,
               member< account_object, account_id_type, &account_object::id >
            > /// composite key by_next_rep_withdrawal
         >,
         ordered_unique< tag< by_last_post >,
            composite_key< account_object,
               member< account_object, time_point_sec, &account_object::last_post >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< time_point_sec >, std::less< account_id_type > >
         >,
         ordered_unique< tag< by_bmt_balance >,
            composite_key< account_object,
               member< account_object, asset, &account_object::balance >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< asset >, std::less< account_id_type > >
         >,
         ordered_unique< tag< by_smp_balance >,
            composite_key< account_object,
               member< account_object, asset, &account_object::rep_shares >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< asset >, std::less< account_id_type > >
         >,
         ordered_unique< tag< by_smd_balance >,
            composite_key< account_object,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::less< account_id_type > >
         >,
         ordered_unique< tag< by_post_count >,
            composite_key< account_object,
               member< account_object, uint32_t, &account_object::post_count >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< uint32_t >, std::less< account_id_type > >
         >,
         ordered_unique< tag< by_vote_count >,
            composite_key< account_object,
               member< account_object, uint32_t, &account_object::lifetime_vote_count >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< uint32_t >, std::less< account_id_type > >
         >,
         ordered_unique< tag< by_rep_shares >,
            composite_key< account_object,
               member< account_object, asset, &account_object::rep_shares >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< asset >, std::less< account_id_type > >
         >
      >,
      allocator< account_object >
   > account_index;

   struct by_account;
   struct by_last_valid;

   typedef multi_index_container <
      owner_authority_history_object,
      indexed_by <
         ordered_unique< tag< by_id >,
            member< owner_authority_history_object, owner_authority_history_id_type, &owner_authority_history_object::id > >,
         ordered_unique< tag< by_account >,
            composite_key< owner_authority_history_object,
               member< owner_authority_history_object, account_name_type, &owner_authority_history_object::account >,
               member< owner_authority_history_object, time_point_sec, &owner_authority_history_object::last_valid_time >,
               member< owner_authority_history_object, owner_authority_history_id_type, &owner_authority_history_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< time_point_sec >, std::less< owner_authority_history_id_type > >
         >
      >,
      allocator< owner_authority_history_object >
   > owner_authority_history_index;

   struct by_last_owner_update;

   typedef multi_index_container <
      account_authority_object,
      indexed_by <
         ordered_unique< tag< by_id >,
            member< account_authority_object, account_authority_id_type, &account_authority_object::id > >,
         ordered_unique< tag< by_account >,
            composite_key< account_authority_object,
               member< account_authority_object, account_name_type, &account_authority_object::account >,
               member< account_authority_object, account_authority_id_type, &account_authority_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< account_authority_id_type > >
         >,
         ordered_unique< tag< by_last_owner_update >,
            composite_key< account_authority_object,
               member< account_authority_object, time_point_sec, &account_authority_object::last_owner_update >,
               member< account_authority_object, account_authority_id_type, &account_authority_object::id >
            >,
            composite_key_compare< std::greater< time_point_sec >, std::less< account_authority_id_type > >
         >
      >,
      allocator< account_authority_object >
   > account_authority_index;

   struct by_delegation;

   typedef multi_index_container <
      rep_delegation_object,
      indexed_by <
         ordered_unique< tag< by_id >,
            member< rep_delegation_object, rep_delegation_id_type, &rep_delegation_object::id > >,
         ordered_unique< tag< by_delegation >,
            composite_key< rep_delegation_object,
               member< rep_delegation_object, account_name_type, &rep_delegation_object::delegator >,
               member< rep_delegation_object, account_name_type, &rep_delegation_object::delegatee >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< account_name_type > >
         >
      >,
      allocator< rep_delegation_object >
   > rep_delegation_index;

   struct by_expiration;
   struct by_account_expiration;

   typedef multi_index_container <
      rep_delegation_expiration_object,
      indexed_by <
         ordered_unique< tag< by_id >,
            member< rep_delegation_expiration_object, rep_delegation_expiration_id_type, &rep_delegation_expiration_object::id > >,
         ordered_unique< tag< by_expiration >,
            composite_key< rep_delegation_expiration_object,
               member< rep_delegation_expiration_object, time_point_sec, &rep_delegation_expiration_object::expiration >,
               member< rep_delegation_expiration_object, rep_delegation_expiration_id_type, &rep_delegation_expiration_object::id >
            >,
            composite_key_compare< std::less< time_point_sec >, std::less< rep_delegation_expiration_id_type > >
         >,
         ordered_unique< tag< by_account_expiration >,
            composite_key< rep_delegation_expiration_object,
               member< rep_delegation_expiration_object, account_name_type, &rep_delegation_expiration_object::delegator >,
               member< rep_delegation_expiration_object, time_point_sec, &rep_delegation_expiration_object::expiration >,
               member< rep_delegation_expiration_object, rep_delegation_expiration_id_type, &rep_delegation_expiration_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< time_point_sec >, std::less< rep_delegation_expiration_id_type > >
         >
      >,
      allocator< rep_delegation_expiration_object >
   > rep_delegation_expiration_index;

   struct by_expiration;

   typedef multi_index_container <
      account_recovery_request_object,
      indexed_by <
         ordered_unique< tag< by_id >,
            member< account_recovery_request_object, account_recovery_request_id_type, &account_recovery_request_object::id > >,
         ordered_unique< tag< by_account >,
            composite_key< account_recovery_request_object,
               member< account_recovery_request_object, account_name_type, &account_recovery_request_object::account_to_recover >,
               member< account_recovery_request_object, account_recovery_request_id_type, &account_recovery_request_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< account_recovery_request_id_type > >
         >,
         ordered_unique< tag< by_expiration >,
            composite_key< account_recovery_request_object,
               member< account_recovery_request_object, time_point_sec, &account_recovery_request_object::expires >,
               member< account_recovery_request_object, account_recovery_request_id_type, &account_recovery_request_object::id >
            >,
            composite_key_compare< std::less< time_point_sec >, std::less< account_recovery_request_id_type > >
         >
      >,
      allocator< account_recovery_request_object >
   > account_recovery_request_index;

   struct by_effective_date;

   typedef multi_index_container <
      change_recovery_account_request_object,
      indexed_by <
         ordered_unique< tag< by_id >,
            member< change_recovery_account_request_object, change_recovery_account_request_id_type, &change_recovery_account_request_object::id > >,
         ordered_unique< tag< by_account >,
            composite_key< change_recovery_account_request_object,
               member< change_recovery_account_request_object, account_name_type, &change_recovery_account_request_object::account_to_recover >,
               member< change_recovery_account_request_object, change_recovery_account_request_id_type, &change_recovery_account_request_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< change_recovery_account_request_id_type > >
         >,
         ordered_unique< tag< by_effective_date >,
            composite_key< change_recovery_account_request_object,
               member< change_recovery_account_request_object, time_point_sec, &change_recovery_account_request_object::effective_on >,
               member< change_recovery_account_request_object, change_recovery_account_request_id_type, &change_recovery_account_request_object::id >
            >,
            composite_key_compare< std::less< time_point_sec >, std::less< change_recovery_account_request_id_type > >
         >
      >,
      allocator< change_recovery_account_request_object >
   > change_recovery_account_request_index;
} }

FC_REFLECT( bmchain::chain::account_object,
             (id)(name)(memo_key)(json_metadata)(proxy)(last_account_update)
             (created)(mined)
             (owner_challenged)(active_challenged)(last_owner_proved)(last_active_proved)(recovery_account)(last_account_recovery)(reset_account)
             (comment_count)(lifetime_vote_count)(post_count)(can_vote)(voting_power)(last_vote_time)
             (balance)
             (savings_balance)
             (savings_withdraw_requests)
             (reward_bmt_balance)(reward_rep_balance)(reward_rep_bmt)
             (rep_shares)(delegated_rep_shares)(received_rep_shares)
             (rep_withdraw_rate)(next_rep_withdrawal)(withdrawn)(to_withdraw)(withdraw_routes)
             (curation_rewards)
             (posting_rewards)
             (proxied_vsf_votes)(witnesses_voted_for)
             (last_post)(last_root_post)(post_bandwidth)
          )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::account_object, bmchain::chain::account_index )

FC_REFLECT( bmchain::chain::account_authority_object,
             (id)(account)(owner)(active)(posting)(last_owner_update)
)
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::account_authority_object, bmchain::chain::account_authority_index )

FC_REFLECT( bmchain::chain::rep_delegation_object,
            (id)(delegator)(delegatee)(rep_shares)(min_delegation_time) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::rep_delegation_object, bmchain::chain::rep_delegation_index )

FC_REFLECT( bmchain::chain::rep_delegation_expiration_object,
            (id)(delegator)(rep_shares)(expiration) )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::rep_delegation_expiration_object, bmchain::chain::rep_delegation_expiration_index )

FC_REFLECT( bmchain::chain::owner_authority_history_object,
             (id)(account)(previous_owner_authority)(last_valid_time)
          )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::owner_authority_history_object, bmchain::chain::owner_authority_history_index )

FC_REFLECT( bmchain::chain::account_recovery_request_object,
             (id)(account_to_recover)(new_owner_authority)(expires)
          )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::account_recovery_request_object, bmchain::chain::account_recovery_request_index )

FC_REFLECT( bmchain::chain::change_recovery_account_request_object,
             (id)(account_to_recover)(recovery_account)(effective_on)
          )
CHAINBASE_SET_INDEX_TYPE( bmchain::chain::change_recovery_account_request_object, bmchain::chain::change_recovery_account_request_index )
