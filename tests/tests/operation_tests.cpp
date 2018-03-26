#ifdef IS_TEST_NET
#include <boost/test/unit_test.hpp>

#include <bmchain/protocol/exceptions.hpp>

#include <bmchain/chain/database.hpp>
#include <bmchain/chain/database_exceptions.hpp>
#include <bmchain/chain/hardfork.hpp>
#include <bmchain/chain/bmchain_objects.hpp>

#include <bmchain/chain/util/reward.hpp>

#include <bmchain/witness/witness_objects.hpp>

#include <fc/crypto/digest.hpp>

#include "../common/database_fixture.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace bmchain;
using namespace bmchain::chain;
using namespace bmchain::protocol;
using fc::string;

BOOST_FIXTURE_TEST_SUITE( operation_tests, clean_database_fixture )

BOOST_AUTO_TEST_CASE( account_create_validate ){
   try
   {

   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( account_create_authorities ){
   try
   {
      BOOST_TEST_MESSAGE( "Testing: account_create_authorities" );

      account_create_operation op;
      op.creator = "alice";
      op.new_account_name = "bob";

      flat_set< account_name_type > auths;
      flat_set< account_name_type > expected;

      BOOST_TEST_MESSAGE( "--- Testing owner authority" );
      op.get_required_owner_authorities( auths );
      BOOST_REQUIRE( auths == expected );

      BOOST_TEST_MESSAGE( "--- Testing active authority" );
      expected.insert( "alice" );
      op.get_required_active_authorities( auths );
      BOOST_REQUIRE( auths == expected );

      BOOST_TEST_MESSAGE( "--- Testing posting authority" );
      expected.clear();
      auths.clear();
      op.get_required_posting_authorities( auths );
      BOOST_REQUIRE( auths == expected );
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( account_create_apply ){
//   try
//   {
//      BOOST_TEST_MESSAGE( "Testing: account_create_apply" );
//
//      signed_transaction tx;
//      private_key_type priv_key = generate_private_key( "alice" );
//
//      const account_object& init = db.get_account( BMCHAIN_INIT_MINER_NAME );
//      asset init_starting_balance = init.balance;
//
//      const auto& gpo = db.get_dynamic_global_properties();
//
//      account_create_operation op;
//
//      op.fee = asset( 100, BMT_SYMBOL );
//      op.new_account_name = "alice";
//      op.creator = BMCHAIN_INIT_MINER_NAME;
//      op.owner = authority( 1, priv_key.get_public_key(), 1 );
//      op.active = authority( 2, priv_key.get_public_key(), 2 );
//      op.memo_key = priv_key.get_public_key();
//      op.json_metadata = "{\"foo\":\"bar\"}";
//
//      BOOST_TEST_MESSAGE( "--- Test normal account creation" );
//      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
//      tx.operations.push_back( op );
//      tx.sign( init_account_priv_key, db.get_chain_id() );
//      tx.validate();
//      db.push_transaction( tx, 0 );
//
//      const account_object& acct = db.get_account( "alice" );
//      const account_authority_object& acct_auth = db.get< account_authority_object, by_account >( "alice" );
//
//      auto vest_shares = gpo.total_rep_shares;
//      auto vests = gpo.total_rep_fund_bmt;
//
//      BOOST_REQUIRE( acct.name == "alice" );
//      BOOST_REQUIRE( acct_auth.owner == authority( 1, priv_key.get_public_key(), 1 ) );
//      BOOST_REQUIRE( acct_auth.active == authority( 2, priv_key.get_public_key(), 2 ) );
//      BOOST_REQUIRE( acct.memo_key == priv_key.get_public_key() );
//      BOOST_REQUIRE( acct.proxy == "" );
//      BOOST_REQUIRE( acct.created == db.head_block_time() );
//      BOOST_REQUIRE( acct.balance.amount.value == ASSET( "0.000 TESTS" ).amount.value );
//      BOOST_REQUIRE( acct.id._id == acct_auth.id._id );
//
//      /// because init_witness has created vesting shares and blocks have been produced, 100 BMT is worth less than 100 vesting shares due to rounding
//      auto init_rep_shares = BMCHAIN_USER_EMISSION_RATE * 1000;
//      BOOST_REQUIRE( acct.rep_shares.amount.value == init_rep_shares + ( op.fee * ( vest_shares / vests ) ).amount.value );
//      BOOST_REQUIRE( acct.rep_withdraw_rate.amount.value == ASSET( "0.000000 VESTS" ).amount.value );
//      BOOST_REQUIRE( acct.proxied_vsf_votes_total().value == 0 );
//      BOOST_REQUIRE( ( init_starting_balance - ASSET( "0.100 TESTS" ) ).amount.value == init.balance.amount.value );
//      validate_database();
//
//      BOOST_TEST_MESSAGE( "--- Test failure of duplicate account creation" );
//      BOOST_REQUIRE_THROW( db.push_transaction( tx, database::skip_transaction_dupe_check ), fc::exception );
//
//      BOOST_REQUIRE( acct.name == "alice" );
//      BOOST_REQUIRE( acct_auth.owner == authority( 1, priv_key.get_public_key(), 1 ) );
//      BOOST_REQUIRE( acct_auth.active == authority( 2, priv_key.get_public_key(), 2 ) );
//      BOOST_REQUIRE( acct.memo_key == priv_key.get_public_key() );
//      BOOST_REQUIRE( acct.proxy == "" );
//      BOOST_REQUIRE( acct.created == db.head_block_time() );
//      BOOST_REQUIRE( acct.balance.amount.value == ASSET( "0.000 BMT " ).amount.value );
//      BOOST_REQUIRE( acct.rep_shares.amount.value == init_rep_shares + ( op.fee * ( vest_shares / vests ) ).amount.value );
//      BOOST_REQUIRE( acct.rep_withdraw_rate.amount.value == ASSET( "0.000000 VESTS" ).amount.value );
//      BOOST_REQUIRE( acct.proxied_vsf_votes_total().value == 0 );
//      BOOST_REQUIRE( ( init_starting_balance - ASSET( "0.100 TESTS" ) ).amount.value == init.balance.amount.value );
//      validate_database();
//
//      BOOST_TEST_MESSAGE( "--- Test failure when creator cannot cover fee" );
//      tx.signatures.clear();
//      tx.operations.clear();
//      op.fee = asset( db.get_account( BMCHAIN_INIT_MINER_NAME ).balance.amount + 1, BMT_SYMBOL );
//      op.new_account_name = "bob";
//      tx.operations.push_back( op );
//      tx.sign( init_account_priv_key, db.get_chain_id() );
//      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), fc::exception );
//      validate_database();
//
//      BOOST_TEST_MESSAGE( "--- Test failure covering witness fee" );
//      generate_block();
//      db_plugin->debug_update( [=]( database& db )
//      {
//         db.modify( db.get_witness_schedule_object(), [&]( witness_schedule_object& wso )
//         {
//            wso.median_props.account_creation_fee = ASSET( "10.000 TESTS" );
//         });
//      });
//      generate_block();
//
//      tx.clear();
//      op.fee = ASSET( "1.000 TESTS" );
//      tx.operations.push_back( op );
//      tx.sign( init_account_priv_key, db.get_chain_id() );
//      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), fc::exception );
//      validate_database();
//   }
//   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE(account_update_validate) {
   try {
       BOOST_TEST_MESSAGE("Testing: account_update_validate");

       ACTORS((alice))

       account_update_operation op;
       op.account = "alice";
       op.posting = authority();
       op.posting->weight_threshold = 1;
       op.posting->add_authorities("abcdefghijklmnopq", 1);

       try {
           op.validate();

           signed_transaction tx;
           tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
           tx.operations.push_back(op);
           tx.sign(alice_private_key, db.get_chain_id());
           db.push_transaction(tx, 0);

           BOOST_FAIL("An exception was not thrown for an invalid account name");
       }
       catch (fc::exception &) {}

       validate_database();
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( account_update_authorities ) {
    try
    {
        BOOST_TEST_MESSAGE("Testing: account_update_authorities");

        ACTORS((alice)(bob))
        private_key_type active_key = generate_private_key("new_key");

        db.modify(db.get<account_authority_object, by_account>("alice"), [&](account_authority_object &a) {
            a.active = authority(1, active_key.get_public_key(), 1);
        });

        account_update_operation op;
        op.account = "alice";
        op.json_metadata = "{\"success\":true}";

        signed_transaction tx;
        tx.operations.push_back(op);
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);

        BOOST_TEST_MESSAGE("  Tests when owner authority is not updated ---");
        BOOST_TEST_MESSAGE("--- Test failure when no signature");
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_missing_active_auth);

        BOOST_TEST_MESSAGE("--- Test failure when wrong signature");
        tx.sign(bob_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_missing_active_auth);

        BOOST_TEST_MESSAGE("--- Test failure when containing additional incorrect signature");
        tx.sign(alice_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_irrelevant_sig);

        BOOST_TEST_MESSAGE("--- Test failure when containing duplicate signatures");
        tx.signatures.clear();
        tx.sign(active_key, db.get_chain_id());
        tx.sign(active_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_duplicate_sig);

        BOOST_TEST_MESSAGE("--- Test success on active key");
        tx.signatures.clear();
        tx.sign(active_key, db.get_chain_id());
        db.push_transaction(tx, 0);

        BOOST_TEST_MESSAGE("--- Test success on owner key alone");
        tx.signatures.clear();
        tx.sign(alice_private_key, db.get_chain_id());
        db.push_transaction(tx, database::skip_transaction_dupe_check);

        BOOST_TEST_MESSAGE("  Tests when owner authority is updated ---");
        BOOST_TEST_MESSAGE("--- Test failure when updating the owner authority with an active key");
        tx.signatures.clear();
        tx.operations.clear();
        op.owner = authority(1, active_key.get_public_key(), 1);
        tx.operations.push_back(op);
        tx.sign(active_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_missing_owner_auth);

        BOOST_TEST_MESSAGE("--- Test failure when owner key and active key are present");
        tx.sign(alice_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_irrelevant_sig);

        BOOST_TEST_MESSAGE("--- Test failure when incorrect signature");
        tx.signatures.clear();
        tx.sign(alice_post_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_missing_owner_auth);

        BOOST_TEST_MESSAGE("--- Test failure when duplicate owner keys are present");
        tx.signatures.clear();
        tx.sign(alice_private_key, db.get_chain_id());
        tx.sign(alice_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_duplicate_sig);

        BOOST_TEST_MESSAGE("--- Test success when updating the owner authority with an owner key");
        tx.signatures.clear();
        tx.sign(alice_private_key, db.get_chain_id());
        db.push_transaction(tx, 0);

        validate_database();
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( account_update_apply ){
    try {
        BOOST_TEST_MESSAGE("Testing: account_update_apply");

        ACTORS((alice))
        private_key_type new_private_key = generate_private_key("new_key");

        BOOST_TEST_MESSAGE("--- Test normal update");

        account_update_operation op;
        op.account = "alice";
        op.owner = authority(1, new_private_key.get_public_key(), 1);
        op.active = authority(2, new_private_key.get_public_key(), 2);
        op.memo_key = new_private_key.get_public_key();
        op.json_metadata = "{\"bar\":\"foo\"}";

        signed_transaction tx;
        tx.operations.push_back(op);
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
        tx.sign(alice_private_key, db.get_chain_id());
        db.push_transaction(tx, 0);

        const account_object &acct = db.get_account("alice");
        const account_authority_object &acct_auth = db.get<account_authority_object, by_account>("alice");

        BOOST_REQUIRE(acct.name == "alice");
        BOOST_REQUIRE(acct_auth.owner == authority(1, new_private_key.get_public_key(), 1));
        BOOST_REQUIRE(acct_auth.active == authority(2, new_private_key.get_public_key(), 2));
        BOOST_REQUIRE(acct.memo_key == new_private_key.get_public_key());

        /* This is being moved out of consensus
        #ifndef IS_LOW_MEM
           BOOST_REQUIRE( acct.json_metadata == "{\"bar\":\"foo\"}" );
        #else
           BOOST_REQUIRE( acct.json_metadata == "" );
        #endif
        */

        validate_database();

        BOOST_TEST_MESSAGE("--- Test failure when updating a non-existent account");
        tx.operations.clear();
        tx.signatures.clear();
        op.account = "bob";
        tx.operations.push_back(op);
        tx.sign(new_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception)
        validate_database();


        BOOST_TEST_MESSAGE("--- Test failure when account authority does not exist");
        tx.clear();
        op = account_update_operation();
        op.account = "alice";
        op.posting = authority();
        op.posting->weight_threshold = 1;
        op.posting->add_authorities("dave", 1);
        tx.operations.push_back(op);
        tx.sign(new_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception);
        validate_database();
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_SUITE_END()
#endif
