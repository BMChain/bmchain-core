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

namespace {

/// Return SMT token object controlled by this account identified by its symbol. Throws assert exception when not found!
inline const custom_token_object& get_controlled_token( const database& db, const account_name_type& control_account, const asset_symbol_type& token_symbol )
{
   const custom_token_object* token = db.find< custom_token_object, by_symbol >( token_symbol );
   // The SMT is supposed to be found.
   FC_ASSERT( token != nullptr, "SMT numerical asset identifier ${token} not found", ("token", token_symbol) );
   // Check against unotherized account trying to access (and alter) token. Unotherized means "other than the one that created the token".
   FC_ASSERT( token->control_account == control_account, "The account ${account} does NOT control the token ${token}",
      ("account", control_account)("token", token_symbol) );

   return *token;
}

}

namespace {

class custom_token_setup_parameters_visitor : public fc::visitor<bool> {
public:
   custom_token_setup_parameters_visitor(custom_token_object &custom_token) : _custom_token(custom_token) {}

   bool operator()(const token_param_allow_voting &allow_voting) {
      _custom_token.allow_voting = allow_voting.value;
      return true;
   }

   bool operator()(const token_param_allow_vesting &allow_vesting) {
      _custom_token.allow_vesting = allow_vesting.value;
      return true;
   }

private:
   custom_token_object &_custom_token;
};

}

const custom_token_object& common_pre_setup_evaluation(
   const database& _db, const asset_symbol_type& symbol, const account_name_type& control_account )
{
   const custom_token_object& token = get_controlled_token( _db, control_account, symbol );

   // Check whether it's not too late to setup emissions operation.
   FC_ASSERT( token.phase < smt_phase::setup_completed, "token pre-setup operation no longer allowed after setup phase is over" );

   return token;
}

struct smt_setup_evaluator_visitor
{
   const custom_token_object &_token;
   database &_db;

   smt_setup_evaluator_visitor(const custom_token_object &token, database &db) : _token(token), _db(db) {}

   typedef void result_type;

   void operator()(const custom_token_capped_generation_policy &capped_generation_policy) const {
      _db.modify(_token, [&](custom_token_object &token) {
         token.capped_generation_policy = capped_generation_policy;
      });
   }
};

void custom_token_create_evaluator::do_apply( const custom_token_create_operation& o ) {
   const auto &by_symbol_idx = _db.get_index<custom_token_index>().indices().get<by_symbol>();
   auto itr = by_symbol_idx.find(boost::make_tuple(o.current_supply.symbol));
   const auto&  acc = _db.get_account(o.control_account);
   //FC_ASSERT( acc != nullptr, "This control account dosn't esixt." );
   FC_ASSERT(acc.balance >= o.custom_token_creation_fee, "Control account dosn't have enough BMT.");
   FC_ASSERT(itr == by_symbol_idx.end(), "Custom token with such symbol exists.");

   _db.create<custom_token_object>([&](custom_token_object &token) {
      token.control_account = o.control_account;
      token.symbol = o.current_supply.symbol;
      token.current_supply = o.current_supply;
      token.reward_fund = asset( 0, o.current_supply.symbol );
      token.generation_time = _db.head_block_time();
   });

   _db.create<account_balance_object>([&](account_balance_object &balance_obj) {
      balance_obj.owner = o.control_account;
      balance_obj.symbol = o.current_supply.symbol;
      balance_obj.balance = o.current_supply;
   });

   _db.modify(acc, [&](account_object &acc_obj) {
      acc_obj.balance -= o.custom_token_creation_fee;
   });
}

void custom_token_transfer_evaluator::do_apply( const custom_token_transfer_operation& o ) {
      const auto& by_owner_idx = _db.get_index<account_balance_index>().indices().get<by_owner>();
   auto itr_from = by_owner_idx.find(boost::make_tuple(o.from));
   FC_ASSERT(itr_from != by_owner_idx.end(), "This account doesn't have enough token #1.");
   FC_ASSERT(itr_from->balance >= o.amount, "This account doesn't have enough token #2.");
   FC_ASSERT(itr_from->symbol == o.amount.symbol, "Wrong symbol.");

   _db.modify(*itr_from, [&](account_balance_object &balance_obj) {
      balance_obj.balance -= o.amount;
   });

   auto itr_to = by_owner_idx.find(boost::make_tuple(o.to));
   if (itr_to == by_owner_idx.end()) {
      _db.create<account_balance_object>([&](account_balance_object &balance_obj) {
         balance_obj.owner = o.to;
         balance_obj.symbol = o.amount.symbol;
         balance_obj.balance = o.amount;
      });
   } else {
      _db.modify(*itr_to, [&](account_balance_object &balance_obj) {
         balance_obj.balance += o.amount;
      });
   }
}

void custom_token_setup_emissions_evaluator::do_apply( const custom_token_setup_emissions_operation& o ) {
   const auto&by_symbol_idx = _db.get_index<custom_token_index>().indices().get<by_symbol>();
   auto itr = by_symbol_idx.find(boost::make_tuple(o.symbol.symbol));
   FC_ASSERT(itr != by_symbol_idx.end(), "Custom token with such symbol don't exists.");

   _db.modify(*itr, [&](custom_token_object &obj) {
      obj.inflation_rate = o.inflation_rate;
      obj.schedule_time = o.schedule_time;
//      obj.emissions_unit = o.emissions_unit;
      obj.interval_count = o.interval_count;
      obj.interval_seconds = o.interval_seconds;
   });
}

void custom_token_set_setup_parameters_evaluator::do_apply( const custom_token_set_setup_parameters_operation& o ) {

   const auto& by_owner_idx = _db.get_index< account_balance_index >().indices().get< by_owner >();

   const custom_token_object& token = common_pre_setup_evaluation(_db, o.symbol.symbol, o.control_account);

   _db.modify( token, [&]( custom_token_object& token )
   {
      custom_token_setup_parameters_visitor visitor(token);

      for (auto& param : o.setup_parameters)
         param.visit(visitor);
   });
}

}}