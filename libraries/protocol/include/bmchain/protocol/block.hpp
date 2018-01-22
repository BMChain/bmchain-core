#pragma once
#include <bmchain/protocol/block_header.hpp>
#include <bmchain/protocol/transaction.hpp>

namespace bmchain { namespace protocol {

   struct signed_block : public signed_block_header
   {
      checksum_type calculate_merkle_root()const;
      vector<signed_transaction> transactions;
   };

} } // bmchain::protocol

FC_REFLECT_DERIVED( bmchain::protocol::signed_block, (bmchain::protocol::signed_block_header), (transactions) )
