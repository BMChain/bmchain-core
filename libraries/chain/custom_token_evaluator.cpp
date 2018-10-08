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
      auto itr = by_symbol_idx.find( boost::make_tuple( o.current_supply.symbol ) );
      auto acc = _db.get_account(o.control_account);
      //FC_ASSERT( acc != nullptr, "This control account dosn't esixt." );
      FC_ASSERT( acc.balance >= o.custom_token_creation_fee, "Control account dosn't have enough BMT." );
      FC_ASSERT( itr == by_symbol_idx.end(), "Custom token with such symbol exists." );

      _db.create< custom_token_object >( [&]( custom_token_object& token ) {
         token.control_account = o.control_account;
         token.symbol          = o.current_supply.symbol;
         token.current_supply  = o.current_supply;
         token.generation_time = _db.head_block_time();
      });

      _db.create< account_balance_object >( [&]( account_balance_object& balance_obj ) {
         balance_obj.owner   = o.control_account;
         balance_obj.symbol  = o.current_supply.symbol;
         balance_obj.balance = o.current_supply;
      });

      _db.modify( acc, [&]( account_object& acc_obj )
      {
         acc_obj.balance -= o.custom_token_creation_fee;
      });
   }

   void custom_token_transfer_evaluator::do_apply( const custom_token_transfer_operation& o ) {
      const auto& by_owner_idx = _db.get_index< account_balance_index >().indices().get< by_owner >();
      auto itr_from = by_owner_idx.find( boost::make_tuple( o.from ) );
      FC_ASSERT( itr_from != by_owner_idx.end(), "This account doesn't have enough token #1." );
      FC_ASSERT( itr_from->balance >= o.amount, "This account doesn't have enough token #2." );
      FC_ASSERT( itr_from->symbol == o.amount.symbol, "Wrong symbol." );

      _db.modify( *itr_from, [&]( account_balance_object& balance_obj )
      {
         balance_obj.balance -= o.amount;
      });

      auto itr_to = by_owner_idx.find( boost::make_tuple( o.to ) );
      if ( itr_to == by_owner_idx.end() ) {
         _db.create< account_balance_object >( [&]( account_balance_object& balance_obj ) {
            balance_obj.owner = o.to;
            balance_obj.symbol = o.amount.symbol;
            balance_obj.balance = o.amount;
         });
      }
      else {
         _db.modify( *itr_to, [&]( account_balance_object& balance_obj )
         {
            balance_obj.balance += o.amount;
         });
      }
   }

   void custom_token_setup_emissions_evaluator::do_apply( const custom_token_setup_emissions_operation& o ) {
      const auto& by_symbol_idx = _db.get_index< custom_token_index >().indices().get< by_symbol >();
      auto itr = by_symbol_idx.find( boost::make_tuple( o.symbol.symbol ) );
      FC_ASSERT( itr == by_symbol_idx.end(), "Custom token with such symbol exists." );

      _db.modify( *itr, [&]( custom_token_object& obj )
      {
         obj.inflation_rate   = o.inflation_rate;
         obj.schedule_time    = o.schedule_time;
         obj.emissions_unit   = o.emissions_unit;
         obj.interval_count   = o.interval_count;
         obj.interval_seconds = o.interval_seconds;
      });
   }

   void custom_token_set_setup_parameters_evaluator::do_apply( const custom_token_set_setup_parameters_operation& o ) {

      const auto& by_owner_idx = _db.get_index< account_balance_index >().indices().get< by_owner >();

   }

}}