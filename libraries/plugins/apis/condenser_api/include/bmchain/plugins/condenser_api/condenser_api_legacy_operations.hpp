#include <bmchain/protocol/operations.hpp>
#include <bmchain/protocol>
#include <bmchain/chain/witness_objects.hpp>

#include <bmchain/plugins/condenser_api/condenser_api_legacy_asset.hpp>

namespace bmchain { namespace plugins { namespace condenser_api {

   template< typename T >
   struct convert_to_legacy_static_variant
   {
      convert_to_legacy_static_variant( T& l_sv ) :
         legacy_sv( l_sv ) {}

      T& legacy_sv;

      typedef void result_type;

      template< typename V >
      void operator()( const V& v ) const
      {
         legacy_sv = v;
      }
   };

   typedef static_variant<
            protocol::comment_payout_beneficiaries
   #ifdef STEEM_ENABLE_SMT
            ,protocol::allowed_vote_assets
   #endif
         > legacy_comment_options_extensions;

   typedef vector< legacy_comment_options_extensions > legacy_comment_options_extensions_type;

   typedef static_variant<
            protocol::pow2,
            protocol::equihash_pow
         > legacy_pow2_work;

   using namespace steem::protocol;

   typedef account_update_operation               legacy_account_update_operation;
   typedef comment_operation                      legacy_comment_operation;
   typedef create_claimed_account_operation       legacy_create_claimed_account_operation;
   typedef delete_comment_operation               legacy_delete_comment_operation;
   typedef vote_operation                         legacy_vote_operation;
   typedef escrow_approve_operation               legacy_escrow_approve_operation;
   typedef escrow_dispute_operation               legacy_escrow_dispute_operation;
   typedef set_withdraw_vesting_route_operation   legacy_set_withdraw_vesting_route_operation;
   typedef witness_set_properties_operation       legacy_witness_set_properties_operation;
   typedef account_witness_vote_operation         legacy_account_witness_vote_operation;
   typedef account_witness_proxy_operation        legacy_account_witness_proxy_operation;
   typedef custom_operation                       legacy_custom_operation;
   typedef custom_json_operation                  legacy_custom_json_operation;
   typedef custom_binary_operation                legacy_custom_binary_operation;
   typedef limit_order_cancel_operation           legacy_limit_order_cancel_operation;
   typedef pow_operation                          legacy_pow_operation;
   typedef report_over_production_operation       legacy_report_over_production_operation;
   typedef request_account_recovery_operation     legacy_request_account_recovery_operation;
   typedef recover_account_operation              legacy_recover_account_operation;
   typedef reset_account_operation                legacy_reset_account_operation;
   typedef set_reset_account_operation            legacy_set_reset_account_operation;
   typedef change_recovery_account_operation      legacy_change_recovery_account_operation;
   typedef cancel_transfer_from_savings_operation legacy_cancel_transfer_from_savings_operation;
   typedef decline_voting_rights_operation        legacy_decline_voting_rights_operation;
   typedef shutdown_witness_operation             legacy_shutdown_witness_operation;
   typedef hardfork_operation                     legacy_hardfork_operation;
   typedef comment_payout_update_operation        legacy_comment_payout_update_operation;

   struct legacy_price
   {
      legacy_price() {}
      legacy_price( const protocol::price& p ) :
         base( legacy_asset::from_asset( p.base ) ),
         quote( legacy_asset::from_asset( p.quote ) )
      {}

      operator price()const { return price( base, quote ); }

      legacy_asset base;
      legacy_asset quote;
   };

   struct api_chain_properties
   {
      api_chain_properties() {}
      api_chain_properties( const chain::chain_properties& c ) :
         account_creation_fee( legacy_asset::from_asset( c.account_creation_fee ) ),
         maximum_block_size( c.maximum_block_size ),
         sbd_interest_rate( c.sbd_interest_rate ),
         account_subsidy_budget( c.account_subsidy_budget ),
         account_subsidy_decay( c.account_subsidy_decay )
      {}

      operator legacy_chain_properties() const
      {
         legacy_chain_properties props;
         props.account_creation_fee = legacy_steem_asset::from_asset( asset( account_creation_fee ) );
         props.maximum_block_size = maximum_block_size;
         props.sbd_interest_rate = sbd_interest_rate;
         return props;
      }

      legacy_asset   account_creation_fee;
      uint32_t       maximum_block_size = STEEM_MIN_BLOCK_SIZE_LIMIT * 2;
      uint16_t       sbd_interest_rate = STEEM_DEFAULT_SBD_INTEREST_RATE;
      int32_t        account_subsidy_budget = STEEM_DEFAULT_ACCOUNT_SUBSIDY_BUDGET;
      uint32_t       account_subsidy_decay = STEEM_DEFAULT_ACCOUNT_SUBSIDY_DECAY;
   };

   struct legacy_account_create_operation
   {
      legacy_account_create_operation() {}
      legacy_account_create_operation( const account_create_operation& op ) :
         fee( legacy_asset::from_asset( op.fee ) ),
         creator( op.creator ),
         new_account_name( op.new_account_name ),
         owner( op.owner ),
         active( op.active ),
         posting( op.posting ),
         memo_key( op.memo_key ),
         json_metadata( op.json_metadata )
      {}

      operator account_create_operation()const
      {
         account_create_operation op;
         op.fee = fee;
         op.creator = creator;
         op.new_account_name = new_account_name;
         op.owner = owner;
         op.active = active;
         op.posting = posting;
         op.memo_key = memo_key;
         op.json_metadata = json_metadata;
         return op;
      }

      legacy_asset      fee;
      account_name_type creator;
      account_name_type new_account_name;
      authority         owner;
      authority         active;
      authority         posting;
      public_key_type   memo_key;
      string            json_metadata;
   };

   struct legacy_account_create_with_delegation_operation
   {
      legacy_account_create_with_delegation_operation() {}
      legacy_account_create_with_delegation_operation( const account_create_with_delegation_operation op ) :
         fee( legacy_asset::from_asset( op.fee ) ),
         delegation( legacy_asset::from_asset( op.delegation ) ),
         creator( op.creator ),
         new_account_name( op.new_account_name ),
         owner( op.owner ),
         active( op.active ),
         posting( op.posting ),
         memo_key( op.memo_key ),
         json_metadata( op.json_metadata )
      {
         extensions.insert( op.extensions.begin(), op.extensions.end() );
      }

      operator account_create_with_delegation_operation()const
      {
         account_create_with_delegation_operation op;
         op.fee = fee;
         op.delegation = delegation;
         op.creator = creator;
         op.new_account_name = new_account_name;
         op.owner = owner;
         op.active = active;
         op.posting = posting;
         op.memo_key = memo_key;
         op.json_metadata = json_metadata;
         op.extensions.insert( extensions.begin(), extensions.end() );
         return op;
      }

      legacy_asset      fee;
      legacy_asset      delegation;
      account_name_type creator;
      account_name_type new_account_name;
      authority         owner;
      authority         active;
      authority         posting;
      public_key_type   memo_key;
      string            json_metadata;

      extensions_type   extensions;
   };

   struct legacy_comment_options_operation
   {
      legacy_comment_options_operation() {}
      legacy_comment_options_operation( const comment_options_operation& op ) :
         author( op.author ),
         permlink( op.permlink ),
         max_accepted_payout( legacy_asset::from_asset( op.max_accepted_payout ) ),
         percent_steem_dollars( op.percent_steem_dollars ),
         allow_votes( op.allow_votes ),
         allow_curation_rewards( op.allow_curation_rewards )
      {
         for( const auto& e : op.extensions )
         {
            legacy_comment_options_extensions ext;
            e.visit( convert_to_legacy_static_variant< legacy_comment_options_extensions >( ext ) );
            extensions.push_back( e );
         }
      }

      operator comment_options_operation()const
      {
         comment_options_operation op;
         op.author = author;
         op.permlink = permlink;
         op.max_accepted_payout = max_accepted_payout;
         op.percent_steem_dollars = percent_steem_dollars;
         op.allow_curation_rewards = allow_curation_rewards;
         op.extensions.insert( extensions.begin(), extensions.end() );
         return op;
      }

      account_name_type author;
      string            permlink;

      legacy_asset      max_accepted_payout;
      uint16_t          percent_steem_dollars;
      bool              allow_votes;
      bool              allow_curation_rewards;
      legacy_comment_options_extensions_type extensions;
   };


   struct legacy_transfer_operation
   {
      legacy_transfer_operation() {}
      legacy_transfer_operation( const transfer_operation& op ) :
         from( op.from ),
         to( op.to ),
         amount( legacy_asset::from_asset( op.amount ) ),
         memo( op.memo )
      {}

      operator transfer_operation()const
      {
         transfer_operation op;
         op.from = from;
         op.to = to;
         op.amount = amount;
         op.memo = memo;
         return op;
      }

      account_name_type from;
      account_name_type to;
      legacy_asset      amount;
      string            memo;
   };

   struct legacy_escrow_transfer_operation
   {
      legacy_escrow_transfer_operation() {}
      legacy_escrow_transfer_operation( const escrow_transfer_operation& op ) :
         from( op.from ),
         to( op.to ),
         agent( op.agent ),
         escrow_id( op.escrow_id ),
         sbd_amount( legacy_asset::from_asset( op.sbd_amount ) ),
         steem_amount( legacy_asset::from_asset( op.steem_amount ) ),
         fee( legacy_asset::from_asset( op.fee ) ),
         ratification_deadline( op.ratification_deadline ),
         escrow_expiration( op.escrow_expiration ),
         json_meta( op.json_meta )
      {}

      operator escrow_transfer_operation()const
      {
         escrow_transfer_operation op;
         op.from = from;
         op.to = to;
         op.agent = agent;
         op.escrow_id = escrow_id;
         op.sbd_amount = sbd_amount;
         op.steem_amount = steem_amount;
         op.fee = fee;
         op.ratification_deadline = ratification_deadline;
         op.escrow_expiration = escrow_expiration;
         op.json_meta = json_meta;
         return op;
      }

      account_name_type from;
      account_name_type to;
      account_name_type agent;
      uint32_t          escrow_id;

      legacy_asset      sbd_amount;
      legacy_asset      steem_amount;
      legacy_asset      fee;

      time_point_sec    ratification_deadline;
      time_point_sec    escrow_expiration;

      string            json_meta;
   };

   struct legacy_escrow_release_operation
   {
      legacy_escrow_release_operation() {}
      legacy_escrow_release_operation( const escrow_release_operation& op ) :
         from( op.from ),
         to( op.to ),
         agent( op.agent ),
         who( op.who ),
         receiver( op.receiver ),
         escrow_id( op.escrow_id ),
         sbd_amount( legacy_asset::from_asset( op.sbd_amount ) ),
         steem_amount( legacy_asset::from_asset( op.steem_amount ) )
      {}

      operator escrow_release_operation()const
      {
         escrow_release_operation op;
         op.from = from;
         op.to = to;
         op.agent = agent;
         op.who = who;
         op.receiver = receiver;
         op.escrow_id = escrow_id;
         op.sbd_amount = sbd_amount;
         op.steem_amount = steem_amount;
         return op;
      }

      account_name_type from;
      account_name_type to;
      account_name_type agent;
      account_name_type who;
      account_name_type receiver;

      uint32_t          escrow_id;
      legacy_asset      sbd_amount;
      legacy_asset      steem_amount;
   };

   struct legacy_pow2_operation
   {
      legacy_pow2_operation() {}
      legacy_pow2_operation( const pow2_operation& op ) :
         new_owner_key( op.new_owner_key )
      {
         op.work.visit( convert_to_legacy_static_variant< legacy_pow2_work >( work ) );
         props.account_creation_fee = legacy_asset::from_asset( op.props.account_creation_fee.to_asset< false >() );
         props.maximum_block_size = op.props.maximum_block_size;
         props.sbd_interest_rate = op.props.sbd_interest_rate;
      }

      operator pow2_operation()const
      {
         pow2_operation op;
         work.visit( convert_to_legacy_static_variant< pow2_work >( op.work ) );
         op.new_owner_key = new_owner_key;
         op.props.account_creation_fee = legacy_steem_asset::from_asset( asset( props.account_creation_fee ) );
         op.props.maximum_block_size = props.maximum_block_size;
         op.props.sbd_interest_rate = props.sbd_interest_rate;
         return op;
      }

      legacy_pow2_work              work;
      optional< public_key_type >   new_owner_key;
      api_chain_properties          props;
   };

   struct legacy_transfer_to_vesting_operation
   {
      legacy_transfer_to_vesting_operation() {}
      legacy_transfer_to_vesting_operation( const transfer_to_vesting_operation& op ) :
         from( op.from ),
         to( op.to ),
         amount( legacy_asset::from_asset( op.amount ) )
      {}

      operator transfer_to_vesting_operation()const
      {
         transfer_to_vesting_operation op;
         op.from = from;
         op.to = to;
         op.amount = amount;
         return op;
      }

      account_name_type from;
      account_name_type to;
      legacy_asset      amount;
   };


} } } // steem::plugins::condenser_api

namespace fc {

void to_variant( const steem::plugins::condenser_api::legacy_operation&, fc::variant& );
void from_variant( const fc::variant&, steem::plugins::condenser_api::legacy_operation& );

void to_variant( const steem::plugins::condenser_api::legacy_comment_options_extensions&, fc::variant& );
void from_variant( const fc::variant&, steem::plugins::condenser_api::legacy_comment_options_extensions& );

void to_variant( const steem::plugins::condenser_api::legacy_pow2_work&, fc::variant& );
void from_variant( const fc::variant&, steem::plugins::condenser_api::legacy_pow2_work& );

struct from_old_static_variant
{
   variant& var;
   from_old_static_variant( variant& dv ):var(dv){}

   typedef void result_type;
   template<typename T> void operator()( const T& v )const
   {
      to_variant( v, var );
   }
};

struct to_old_static_variant
{
   const variant& var;
   to_old_static_variant( const variant& dv ):var(dv){}

   typedef void result_type;
   template<typename T> void operator()( T& v )const
   {
      from_variant( var, v );
   }
};

template< typename T >
void old_sv_to_variant( const T& sv, fc::variant& v )
{
   variant tmp;
   variants vars(2);
   vars[0] = sv.which();
   sv.visit( from_old_static_variant(vars[1]) );
   v = std::move(vars);
}

