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
      asset             current_supply;
      asset             custom_token_creation_fee;

      void validate() const;
      void get_required_owner_authorities( flat_set<account_name_type>& a ) const { a.insert(control_account); }
   };

   struct custom_token_transfer_operation : public base_operation
   {
      account_name_type from;
      account_name_type to;
      asset             amount;

      void              validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a ) const { a.insert(from); }
      void get_required_owner_authorities( flat_set<account_name_type>& a ) const { a.insert(from); }
   };

   struct custom_token_setup_emissions_operation : public base_operation
   {
      custom_token_emissions_unit emissions_unit;
      time_point_sec       schedule_time;

      account_name_type control_account;
      asset             symbol;
      uint16_t          inflation_rate;

      uint32_t          interval_seconds = 0;
      uint32_t          interval_count = 0;

      void              validate()const;
      void get_required_owner_authorities( flat_set<account_name_type>& a ) const { a.insert(control_account); }
   };

   struct custom_token_capped_generation_policy
   {
//         smt_generation_unit pre_soft_cap_unit;
//         smt_generation_unit post_soft_cap_unit;
//
//         smt_cap_commitment  min_steem_units_commitment;
//         smt_cap_commitment  hard_cap_steem_units_commitment;

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
         asset             symbol;

         uint8_t                 decimal_places = 0;
         int64_t                 max_supply = BMCHAIN_MAX_SHARE_SUPPLY;

         smt_generation_policy   initial_generation_policy;

         time_point_sec          generation_begin_time;
         time_point_sec          generation_end_time;
         time_point_sec          announced_launch_time;
         time_point_sec          launch_expiration_time;

         extensions_type         extensions;

         void validate()const;
   };

   struct custom_token_set_setup_parameters_operation : public base_operation
   {
      //flat_set< smt_setup_parameter >  setup_parameters;
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