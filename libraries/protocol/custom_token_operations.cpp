#include <bmchain/protocol/custom_token_operations.hpp>
#include <fc/io/json.hpp>

#include <locale>

namespace bmchain { namespace protocol {

void custom_token_create_operation::validate() const {}

void custom_token_transfer_operation::validate() const {}

void custom_token_setup_emissions_operation::validate() const {
   FC_ASSERT( schedule_time > BMCHAIN_GENESIS_TIME );
//   FC_ASSERT( emissions_unit.token_unit.empty() == false );
}

void custom_token_set_setup_parameters_operation::validate() const {}

void custom_token_setup_operation::validate() const {}

void custom_token_capped_generation_policy::validate() const {}

void custom_token_cap_commitment::validate() const {}

void custom_token_generation_unit::validate() const {}

struct smt_set_runtime_parameters_operation_visitor
{
   smt_set_runtime_parameters_operation_visitor(){}

   typedef void result_type;

   void operator()( const token_param_windows_v1& param_windows ) const
   {
      uint64_t sum = ( param_windows.reverse_auction_window_seconds + CUSTOM_TOKEN_UPVOTE_LOCKOUT );

      FC_ASSERT( param_windows.cashout_window_seconds > ( sum ),
               "Cashout window must be greater than 'reverse auction window + upvote lockout' interval. This interval is ${sum} minutes long.",
               ( "sum", sum/60 ) );

      FC_ASSERT( param_windows.cashout_window_seconds < CUSTOM_TOKEN_VESTING_WITHDRAW_INTERVAL_SECONDS,
               "Cashout window second must be less than 'vesting withdraw' interval. This interval is ${val} minutes long.",
               ("val", CUSTOM_TOKEN_VESTING_WITHDRAW_INTERVAL_SECONDS/60 ) );
   }

   void operator()( const token_param_vote_regeneration_period_seconds_v1& vote_regeneration ) const
   {
      FC_ASSERT( ( vote_regeneration.vote_regeneration_period_seconds > 0 ) &&
                 ( vote_regeneration.vote_regeneration_period_seconds < CUSTOM_TOKEN_VESTING_WITHDRAW_INTERVAL_SECONDS ),
               "Vote regeneration period must be greater than 0 and less than 'vesting withdraw' interval. This interval is ${val} minutes long.",
               ("val", CUSTOM_TOKEN_VESTING_WITHDRAW_INTERVAL_SECONDS/60 ) );
   }

   void operator()( const token_param_rewards_v1& param_rewards ) const
   {
      //Nothing to do.
   }
};

}}