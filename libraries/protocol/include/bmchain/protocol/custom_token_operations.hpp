#pragma once
#include <bmchain/protocol/base.hpp>
#include <bmchain/protocol/block_header.hpp>
#include <bmchain/protocol/asset.hpp>

#include <fc/utf8.hpp>
#include <fc/crypto/equihash.hpp>

namespace bmchain { namespace protocol {

struct custom_token_emissions_unit
{
   flat_map< account_name_type, uint16_t > token_unit;
};

struct custom_token_create_operation : public base_operation
{
   account_name_type control_account;
   asset current_supply;
   asset custom_token_creation_fee;

   void validate() const;

   void get_required_owner_authorities( flat_set<account_name_type>& a ) const { a.insert(control_account); }
};

struct custom_token_transfer_operation : public base_operation
{
   account_name_type from;
   account_name_type to;
   asset amount;

   void validate() const;

   void get_required_active_authorities(flat_set<account_name_type> &a) const { a.insert(from); }

   void get_required_owner_authorities( flat_set<account_name_type>& a ) const { a.insert(from); }
};

struct custom_token_setup_emissions_operation : public base_operation
{
   custom_token_emissions_unit emissions_unit;
   time_point_sec schedule_time;

   account_name_type control_account;
   asset symbol;
   uint16_t inflation_rate;

   uint32_t interval_seconds = 0;
   uint32_t interval_count = 0;

   void validate() const;

   void get_required_owner_authorities( flat_set<account_name_type>& a ) const { a.insert(control_account); }
};

struct custom_token_generation_unit
{
   flat_map< account_name_type, uint16_t >        bmt_unit;
   flat_map< account_name_type, uint16_t >        token_unit;

   uint32_t steem_unit_sum()const;
   uint32_t token_unit_sum()const;

   void validate()const;
};

struct custom_token_cap_commitment
{
   share_type            lower_bound;
   share_type            upper_bound;
   digest_type           hash;

   void validate()const;

   // helper to get what the hash should be when lower_bound == upper_bound and nonce == 0
   static void fillin_nonhidden_value_hash( fc::sha256& result, share_type amount );
   // like fillin_nonhidden_value_hash, but returns a new object instead of modify-in-place
   static fc::sha256 get_nonhidden_value_hash( share_type amount )
   {
      fc::sha256 h;
      fillin_nonhidden_value_hash( h, amount );
      return h;
   }

   // helper to fill in the fields so that lower_bound == upper_bound and nonce == 0
   void fillin_nonhidden_value( share_type amount );
   // like fillin_nonhidden_value, but returns a new object instead of modify-in-place
   static custom_token_cap_commitment get_nonhidden_value( share_type amount )
   {
      custom_token_cap_commitment c;
      c.fillin_nonhidden_value( amount );
      return c;
   }
};


struct custom_token_capped_generation_policy
{
   custom_token_generation_unit pre_soft_cap_unit;
   custom_token_generation_unit post_soft_cap_unit;

   custom_token_cap_commitment  min_steem_units_commitment;
   custom_token_cap_commitment  hard_cap_steem_units_commitment;

   uint16_t            soft_cap_percent = 0;

   uint32_t            min_unit_ratio = 0;
   uint32_t            max_unit_ratio = 0;

   extensions_type     extensions;

   void validate()const;
};

typedef static_variant<
        custom_token_capped_generation_policy
> smt_generation_policy;

struct custom_token_setup_operation : public base_operation
{
   account_name_type control_account;
   asset symbol;

   uint8_t decimal_places = 0;
   int64_t max_supply = BMCHAIN_MAX_SHARE_SUPPLY;

   smt_generation_policy initial_generation_policy;

   time_point_sec generation_begin_time;
   time_point_sec generation_end_time;
   time_point_sec announced_launch_time;
   time_point_sec launch_expiration_time;

   extensions_type extensions;

   void validate()const;
};

struct token_param_allow_vesting
{
   bool value = true;
};

struct token_param_allow_voting
{
   bool value = true;
};

struct token_param_windows_v1
{
   uint32_t cashout_window_seconds = 0;                // BMCHAIN_CASHOUT_WINDOW_SECONDS
   uint32_t reverse_auction_window_seconds = 0;        // BMCHAIN_REVERSE_AUCTION_WINDOW_SECONDS
};

struct token_param_vote_regeneration_period_seconds_v1
{
   uint32_t vote_regeneration_period_seconds = 0;      // BMCHAIN_VOTE_REGENERATION_SECONDS
   uint32_t votes_per_regeneration_period = 0;
};

struct token_param_rewards_v1
{
         uint128_t               content_constant = 0;
         uint16_t                percent_curation_rewards = 0;
         uint16_t                percent_content_rewards = 0;
//         protocol::curve_id                author_reward_curve;
//         protocol::curve_id                curation_reward_curve;
};

typedef static_variant<
              token_param_allow_vesting,
        token_param_allow_voting
> token_setup_parameter;

struct custom_token_set_setup_parameters_operation : public base_operation
{
      flat_set< token_setup_parameter >  setup_parameters;
      extensions_type                  extensions;

      void validate()const;
};

}} // bmchain::protocol

FC_REFLECT( bmchain::protocol::custom_token_emissions_unit, (token_unit))
FC_REFLECT( bmchain::protocol::custom_token_create_operation,(control_account)(current_supply)(custom_token_creation_fee) )
FC_REFLECT( bmchain::protocol::custom_token_transfer_operation,(from)(to)(amount) )
FC_REFLECT( bmchain::protocol::custom_token_setup_emissions_operation,
            (emissions_unit)(schedule_time)(control_account)(symbol)(inflation_rate)(interval_seconds)(interval_count) )
FC_REFLECT( bmchain::protocol::custom_token_set_setup_parameters_operation,(extensions) )
FC_REFLECT( bmchain::protocol::custom_token_generation_unit, (bmt_unit)(token_unit))
FC_REFLECT( bmchain::protocol::custom_token_cap_commitment, (lower_bound)(upper_bound)(hash))
FC_REFLECT( bmchain::protocol::custom_token_capped_generation_policy,(pre_soft_cap_unit)(post_soft_cap_unit)
                (min_steem_units_commitment)(hard_cap_steem_units_commitment)(soft_cap_percent)
                (min_unit_ratio)(max_unit_ratio)(extensions))
FC_REFLECT( bmchain::protocol::custom_token_setup_operation,(decimal_places)(max_supply)(initial_generation_policy)
        (generation_begin_time)(generation_end_time)(announced_launch_time)(launch_expiration_time)(extensions))