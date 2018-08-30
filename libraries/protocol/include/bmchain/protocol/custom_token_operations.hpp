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
      void get_required_posting_authorities( flat_set<account_name_type>& a )const{ a.insert(control_account); }
   };

}} // bmchain::protocol

FC_REFLECT( bmchain::protocol::custom_token_create_operation,(control_account)(symbol)(current_supply) )