#include <bmchain/chain/bmchain_evaluator.hpp>
#include <bmchain/chain/database.hpp>
#include <bmchain/chain/custom_operation_interpreter.hpp>
#include <bmchain/chain/bmchain_objects.hpp>
#include <bmchain/chain/witness_objects.hpp>
#include <bmchain/chain/block_summary_object.hpp>
#include <bmchain/private_message/private_message_evaluators.hpp>
#include <bmchain/private_message/private_message_operations.hpp>
#include <bmchain/private_message/private_message_plugin.hpp>
#include <locale>

#include <bmchain/chain/util/reward.hpp>

#include <bmchain/chain/custom_token_objects.hpp>

namespace bmchain { namespace chain {

   void custom_token_create_evaluator::do_apply( const custom_token_create_operation& o ) {
      const auto& by_symbol_idx = _db.get_index< custom_token_index >().indices().get< by_symbol >();
      auto itr = by_symbol_idx.find( boost::make_tuple( o.symbol ) );
      FC_ASSERT( itr == by_symbol_idx.end(), "Custom token with such symbol exists." );

      _db.create< custom_token_object >( [&]( custom_token_object& token ) {
         token.control_account = o.control_account;
         from_string( token.symbol, o.symbol );
         token.current_supply = o.current_supply;
         token.generation_time = _db.head_block_time();
      });

      _db.create< account_balance_object >( [&]( account_balance_object& balance_obj ) {
         balance_obj.owner = o.control_account;
         from_string( balance_obj.symbol, o.symbol );
         balance_obj.balance = o.current_supply;
      });
   }

   void custom_token_transfer_evaluator::do_apply( const custom_token_transfer_operation& o ) {

   }

}}