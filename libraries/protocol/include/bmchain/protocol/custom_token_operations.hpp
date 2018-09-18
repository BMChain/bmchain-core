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
      string            symbol;
      uint64_t          current_supply;

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

   struct custom_token_setup_operation : public base_operation
   {
      account_name_type control_account;
      string            symbol;
      uint16_t          inflation_rate;

      void              validate()const;
      void get_required_owner_authorities( flat_set<account_name_type>& a ) const { a.insert(control_account); }
   };

}} // bmchain::protocol

FC_REFLECT( bmchain::protocol::custom_token_create_operation,(control_account)(symbol)(current_supply) )
FC_REFLECT( bmchain::protocol::custom_token_transfer_operation,(from)(to)(amount) )
FC_REFLECT( bmchain::protocol::custom_token_setup_operation,(control_account)(symbol)(inflation_rate) )