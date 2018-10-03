#pragma once
#include <bmchain/protocol/base.hpp>
#include <bmchain/protocol/block_header.hpp>
#include <bmchain/protocol/asset.hpp>

#include <fc/utf8.hpp>
#include <fc/crypto/equihash.hpp>

namespace bmchain { namespace protocol {

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
      account_name_type control_account;
      asset             symbol;
      uint16_t          inflation_rate;
      time_point_sec    schedule_time;
      uint32_t          interval_seconds = 0;
      uint32_t          interval_count = 0;

      void              validate()const;
      void get_required_owner_authorities( flat_set<account_name_type>& a ) const { a.insert(control_account); }
   };

   struct custom_token_set_setup_parameters_operation : public base_operation
   {
      //flat_set< smt_setup_parameter >  setup_parameters;
      extensions_type                  extensions;

      void validate()const;
   };

}} // bmchain::protocol

FC_REFLECT( bmchain::protocol::custom_token_create_operation,(control_account)(current_supply)(custom_token_creation_fee) )
FC_REFLECT( bmchain::protocol::custom_token_transfer_operation,(from)(to)(amount) )
FC_REFLECT( bmchain::protocol::custom_token_setup_emissions_operation,
            (control_account)(symbol)(inflation_rate)(schedule_time)(interval_seconds)(interval_count) )
FC_REFLECT( bmchain::protocol::custom_token_set_setup_parameters_operation,(extensions) )