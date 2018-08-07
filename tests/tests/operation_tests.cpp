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

//#include <fc/macros.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace bmchain;
using namespace bmchain::chain;
using namespace bmchain::protocol;
using fc::string;

BOOST_FIXTURE_TEST_SUITE( operation_tests, clean_database_fixture )

BOOST_AUTO_TEST_CASE( account_create_validate ) {
   try
   {

   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( account_create_authorities ) {
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

BOOST_AUTO_TEST_CASE( account_create_apply ) {
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

BOOST_AUTO_TEST_CASE( account_update_apply ) {
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

BOOST_AUTO_TEST_CASE( comment_validate ) {
    try {
        BOOST_TEST_MESSAGE("Testing: comment_validate");


        validate_database();
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( comment_authorities ) {
    try
    {
        BOOST_TEST_MESSAGE("Testing: comment_authorities");

        ACTORS((alice)(bob));
        generate_blocks(60 / BMCHAIN_BLOCK_INTERVAL);

        comment_operation op;
        op.author = "alice";
        op.permlink = "lorem";
        op.parent_author = "";
        op.parent_permlink = "ipsum";
        op.title = "Lorem Ipsum";
        op.body = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
        op.json_metadata = "{\"foo\":\"bar\"}";

        signed_transaction tx;
        tx.operations.push_back(op);
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);

        BOOST_TEST_MESSAGE("--- Test failure when no signatures");
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_missing_posting_auth);

        BOOST_TEST_MESSAGE("--- Test failure when duplicate signatures");
        tx.sign(alice_post_key, db.get_chain_id());
        tx.sign(alice_post_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_duplicate_sig);

        BOOST_TEST_MESSAGE("--- Test success with post signature");
        tx.signatures.clear();
        tx.sign(alice_post_key, db.get_chain_id());
        db.push_transaction(tx, 0);

        BOOST_TEST_MESSAGE("--- Test failure when signed by an additional signature not in the creator's authority");
        tx.sign(bob_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, database::skip_transaction_dupe_check), tx_irrelevant_sig);

        BOOST_TEST_MESSAGE("--- Test failure when signed by a signature not in the creator's authority");
        tx.signatures.clear();
        tx.sign(bob_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, database::skip_transaction_dupe_check), tx_missing_posting_auth);

        validate_database();
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( comment_apply ) {
//    try {
//        BOOST_TEST_MESSAGE("Testing: comment_apply");
//
//        ACTORS((alice)(bob)(sam))
//        generate_blocks(60 / BMCHAIN_BLOCK_INTERVAL);
//
//        comment_operation op;
//        op.author = "alice";
//        op.permlink = "lorem";
//        op.parent_author = "";
//        op.parent_permlink = "ipsum";
//        op.title = "Lorem Ipsum";
//        op.body = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
//        op.json_metadata = "{\"foo\":\"bar\"}";
//
//        signed_transaction tx;
//        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
//
//        BOOST_TEST_MESSAGE("--- Test Alice posting a root comment");
//        tx.operations.push_back(op);
//        tx.sign(alice_private_key, db.get_chain_id());
//        db.push_transaction(tx, 0);
//
//        const comment_object &alice_comment = db.get_comment("alice", string("lorem"));
//
//        BOOST_REQUIRE(alice_comment.author == op.author);
//        BOOST_REQUIRE(to_string(alice_comment.permlink) == op.permlink);
//        BOOST_REQUIRE(to_string(alice_comment.parent_permlink) == op.parent_permlink);
//        BOOST_REQUIRE(alice_comment.last_update == db.head_block_time());
//        BOOST_REQUIRE(alice_comment.created == db.head_block_time());
//        BOOST_REQUIRE(alice_comment.net_rshares.value == 0);
//        BOOST_REQUIRE(alice_comment.abs_rshares.value == 0);
//        BOOST_REQUIRE(alice_comment.cashout_time ==
//                      fc::time_point_sec(db.head_block_time() + fc::seconds(BMCHAIN_CASHOUT_WINDOW_SECONDS)));
//
//        validate_database();
//
//        BOOST_TEST_MESSAGE("--- Test Bob posting a comment on a non-existent comment");
//        op.author = "bob";
//        op.permlink = "ipsum";
//        op.parent_author = "alice";
//        op.parent_permlink = "foobar";
//
//        tx.signatures.clear();
//        tx.operations.clear();
//        tx.operations.push_back(op);
//        tx.sign(bob_private_key, db.get_chain_id());
//        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception);
//
//        BOOST_TEST_MESSAGE("--- Test Bob posting a comment on Alice's comment");
//        op.parent_permlink = "lorem";
//
//        tx.signatures.clear();
//        tx.operations.clear();
//        tx.operations.push_back(op);
//        tx.sign(bob_private_key, db.get_chain_id());
//        db.push_transaction(tx, 0);
//
//        const comment_object &bob_comment = db.get_comment("bob", string("ipsum"));
//
//        BOOST_REQUIRE(bob_comment.author == op.author);
//        BOOST_REQUIRE(to_string(bob_comment.permlink) == op.permlink);
//        BOOST_REQUIRE(bob_comment.parent_author == op.parent_author);
//        BOOST_REQUIRE(to_string(bob_comment.parent_permlink) == op.parent_permlink);
//        BOOST_REQUIRE(bob_comment.last_update == db.head_block_time());
//        BOOST_REQUIRE(bob_comment.created == db.head_block_time());
//        BOOST_REQUIRE(bob_comment.net_rshares.value == 0);
//        BOOST_REQUIRE(bob_comment.abs_rshares.value == 0);
//        BOOST_REQUIRE(bob_comment.cashout_time == bob_comment.created + BMCHAIN_CASHOUT_WINDOW_SECONDS);
//        BOOST_REQUIRE(bob_comment.root_comment == alice_comment.id);
//        validate_database();
//
//        BOOST_TEST_MESSAGE("--- Test Sam posting a comment on Bob's comment");
//
//        op.author = "sam";
//        op.permlink = "dolor";
//        op.parent_author = "bob";
//        op.parent_permlink = "ipsum";
//
//        tx.signatures.clear();
//        tx.operations.clear();
//        tx.operations.push_back(op);
//        tx.sign(sam_private_key, db.get_chain_id());
//        db.push_transaction(tx, 0);
//
//        const comment_object &sam_comment = db.get_comment("sam", string("dolor"));
//
//        BOOST_REQUIRE(sam_comment.author == op.author);
//        BOOST_REQUIRE(to_string(sam_comment.permlink) == op.permlink);
//        BOOST_REQUIRE(sam_comment.parent_author == op.parent_author);
//        BOOST_REQUIRE(to_string(sam_comment.parent_permlink) == op.parent_permlink);
//        BOOST_REQUIRE(sam_comment.last_update == db.head_block_time());
//        BOOST_REQUIRE(sam_comment.created == db.head_block_time());
//        BOOST_REQUIRE(sam_comment.net_rshares.value == 0);
//        BOOST_REQUIRE(sam_comment.abs_rshares.value == 0);
//        BOOST_REQUIRE(sam_comment.cashout_time == sam_comment.created + BMCHAIN_CASHOUT_WINDOW_SECONDS);
//        BOOST_REQUIRE(sam_comment.root_comment == alice_comment.id);
//        validate_database();
//
//        generate_blocks(60 * 5 / BMCHAIN_BLOCK_INTERVAL + 1);
//
//        BOOST_TEST_MESSAGE("--- Test modifying a comment");
//        const auto &mod_sam_comment = db.get_comment("sam", string("dolor"));
//        const auto &mod_bob_comment = db.get_comment("bob", string("ipsum"));
//        const auto &mod_alice_comment = db.get_comment("alice", string("lorem"));
//
//        FC_UNUSED(mod_bob_comment, mod_alice_comment);
//
//        fc::time_point_sec created = mod_sam_comment.created;
//
//        db.modify(mod_sam_comment, [&](comment_object &com) {
//            com.net_rshares = 10;
//            com.abs_rshares = 10;
//        });
//
//        db.modify(db.get_dynamic_global_properties(), [&](dynamic_global_property_object &o) {
//            o.total_reward_shares2 = bmchain::chain::util::evaluate_reward_curve(10);
//        });
//
//        tx.signatures.clear();
//        tx.operations.clear();
//        op.title = "foo";
//        op.body = "bar";
//        op.json_metadata = "{\"bar\":\"foo\"}";
//        tx.operations.push_back(op);
//        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
//        tx.sign(sam_private_key, db.get_chain_id());
//        db.push_transaction(tx, 0);
//
//        BOOST_REQUIRE(mod_sam_comment.author == op.author);
//        BOOST_REQUIRE(to_string(mod_sam_comment.permlink) == op.permlink);
//        BOOST_REQUIRE(mod_sam_comment.parent_author == op.parent_author);
//        BOOST_REQUIRE(to_string(mod_sam_comment.parent_permlink) == op.parent_permlink);
//        BOOST_REQUIRE(mod_sam_comment.last_update == db.head_block_time());
//        BOOST_REQUIRE(mod_sam_comment.created == created);
//        BOOST_REQUIRE(mod_sam_comment.cashout_time == mod_sam_comment.created + BMCHAIN_CASHOUT_WINDOW_SECONDS);
//        validate_database();
//
//        BOOST_TEST_MESSAGE("--- Test failure posting withing 1 minute");
//
//        op.permlink = "sit";
//        op.parent_author = "";
//        op.parent_permlink = "test";
//        tx.operations.clear();
//        tx.signatures.clear();
//        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
//        tx.operations.push_back(op);
//        tx.sign(sam_private_key, db.get_chain_id());
//        db.push_transaction(tx, 0);
//
//        generate_blocks(60 * 5 / BMCHAIN_BLOCK_INTERVAL);
//
//        op.permlink = "amet";
//        tx.operations.clear();
//        tx.signatures.clear();
//        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
//        tx.operations.push_back(op);
//        tx.sign(sam_private_key, db.get_chain_id());
//        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception);
//
//        validate_database();
//
//        generate_block();
//        db.push_transaction(tx, 0);
//        validate_database();
//    }
//    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( comment_delete_apply ) {
//    try
//    {
//        BOOST_TEST_MESSAGE("Testing: comment_delete_apply");
//        ACTORS((alice))
//        generate_block();
//
//        vest("alice", ASSET("1000.000 TESTS"));
//
//        generate_block();
//
////        set_price_feed(price(ASSET("1.000 TBD"), ASSET("1.000 TESTS")));
//
//        signed_transaction tx;
//        comment_operation comment;
//        vote_operation vote;
//
//        comment.author = "alice";
//        comment.permlink = "test1";
//        comment.title = "test";
//        comment.body = "foo bar";
//        comment.parent_permlink = "test";
//        vote.voter = "alice";
//        vote.author = "alice";
//        vote.permlink = "test1";
//        vote.weight = BMCHAIN_100_PERCENT;
//        tx.operations.push_back(comment);
//        tx.operations.push_back(vote);
//        tx.set_expiration(db.head_block_time() + BMCHAIN_MIN_TRANSACTION_EXPIRATION_LIMIT);
//        tx.sign(alice_private_key, db.get_chain_id());
//        db.push_transaction(tx, 0);
//
//        BOOST_TEST_MESSAGE("--- Test failue deleting a comment with positive rshares");
//
//        delete_comment_operation op;
//        op.author = "alice";
//        op.permlink = "test1";
//        tx.clear();
//        tx.operations.push_back(op);
//        tx.sign(alice_private_key, db.get_chain_id());
//        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::assert_exception);
//
//
//        BOOST_TEST_MESSAGE("--- Test success deleting a comment with negative rshares");
//
//        generate_block();
//        vote.weight = -1 * BMCHAIN_100_PERCENT;
//        tx.clear();
//        tx.operations.push_back(vote);
//        tx.operations.push_back(op);
//        tx.sign(alice_private_key, db.get_chain_id());
//        db.push_transaction(tx, 0);
//
//        auto test_comment = db.find<comment_object, by_permlink>(boost::make_tuple("alice", string("test1")));
//        BOOST_REQUIRE(test_comment == nullptr);
//
//
//        BOOST_TEST_MESSAGE("--- Test failure deleting a comment past cashout");
//        generate_blocks(BMCHAIN_MIN_ROOT_COMMENT_INTERVAL.to_seconds() / BMCHAIN_BLOCK_INTERVAL);
//
//        tx.clear();
//        tx.operations.push_back(comment);
//        tx.set_expiration(db.head_block_time() + BMCHAIN_MIN_TRANSACTION_EXPIRATION_LIMIT);
//        tx.sign(alice_private_key, db.get_chain_id());
//        db.push_transaction(tx, 0);
//
//        generate_blocks(BMCHAIN_CASHOUT_WINDOW_SECONDS / BMCHAIN_BLOCK_INTERVAL);
////        auto comment1 = db.get_comment("alice", string("test1"));
////        BOOST_REQUIRE(db.get_comment("alice", string("test1")).cashout_time == fc::time_point_sec::maximum());
//
//        tx.clear();
//        tx.operations.push_back(op);
//        tx.set_expiration(db.head_block_time() + BMCHAIN_MIN_TRANSACTION_EXPIRATION_LIMIT);
//        tx.sign(alice_private_key, db.get_chain_id());
//        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::assert_exception);
//
//
//        BOOST_TEST_MESSAGE("--- Test failure deleting a comment with a reply");
//
//        comment.permlink = "test2";
//        comment.parent_author = "alice";
//        comment.parent_permlink = "test1";
//        tx.clear();
//        tx.operations.push_back(comment);
//        tx.set_expiration(db.head_block_time() + BMCHAIN_MIN_TRANSACTION_EXPIRATION_LIMIT);
//        tx.sign(alice_private_key, db.get_chain_id());
//        db.push_transaction(tx, 0);
//
//        generate_blocks(BMCHAIN_MIN_ROOT_COMMENT_INTERVAL.to_seconds() / BMCHAIN_BLOCK_INTERVAL);
//        comment.permlink = "test3";
//        comment.parent_permlink = "test2";
//        tx.clear();
//        tx.operations.push_back(comment);
//        tx.set_expiration(db.head_block_time() + BMCHAIN_MIN_TRANSACTION_EXPIRATION_LIMIT);
//        tx.sign(alice_private_key, db.get_chain_id());
//        db.push_transaction(tx, 0);
//
//        op.permlink = "test2";
//        tx.clear();
//        tx.operations.push_back(op);
//        tx.sign(alice_private_key, db.get_chain_id());
//        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::assert_exception);
//    }
//    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( vote_validate ) {
    try
    {
        BOOST_TEST_MESSAGE("Testing: vote_validate");

        validate_database();
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( vote_authorities ) {
    try
    {
        BOOST_TEST_MESSAGE("Testing: vote_authorities");

        validate_database();
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( vote_apply ) {
    try
    {
        BOOST_TEST_MESSAGE("Testing: vote_apply");

        ACTORS((alice)(bob)(sam)(dave))
        generate_block();

        vest("alice", ASSET("10.000 TESTS"));
        validate_database();
        vest("bob", ASSET("10.000 TESTS"));
        vest("sam", ASSET("10.000 TESTS"));
        vest("dave", ASSET("10.000 TESTS"));
        generate_block();

        const auto &vote_idx = db.get_index<comment_vote_index>().indices().get<by_comment_voter>();

        {
            const auto &alice = db.get_account("alice");

            signed_transaction tx;
            comment_operation comment_op;
            comment_op.author = "alice";
            comment_op.permlink = "foo";
            comment_op.parent_permlink = "test";
            comment_op.title = "bar";
            comment_op.body = "foo bar";
            tx.operations.push_back(comment_op);
            tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
            tx.sign(alice_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);

            BOOST_TEST_MESSAGE("--- Testing voting on a non-existent comment");

            tx.operations.clear();
            tx.signatures.clear();

            vote_operation op;
            op.voter = "alice";
            op.author = "bob";
            op.permlink = "foo";
            op.weight = BMCHAIN_100_PERCENT;
            tx.operations.push_back(op);
            tx.sign(alice_private_key, db.get_chain_id());

            BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception);

            validate_database();

            BOOST_TEST_MESSAGE("--- Testing voting with a weight of 0");

            op.weight = (int16_t) 0;
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(alice_private_key, db.get_chain_id());

            BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception);

            validate_database();

            BOOST_TEST_MESSAGE("--- Testing success");

            auto old_voting_power = alice.voting_power;

            op.weight = BMCHAIN_100_PERCENT;
            op.author = "alice";
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(alice_private_key, db.get_chain_id());

            db.push_transaction(tx, 0);

            auto &alice_comment = db.get_comment("alice", string("foo"));
            auto itr = vote_idx.find(std::make_tuple(alice_comment.id, alice.id));
            int64_t max_vote_denom =
                    (db.get_dynamic_global_properties().vote_power_reserve_rate * BMCHAIN_VOTE_REGENERATION_SECONDS) /
                    (60 * 60 * 24);

            BOOST_REQUIRE(alice.voting_power ==
                          old_voting_power - ((old_voting_power + max_vote_denom - 1) / max_vote_denom));
            BOOST_REQUIRE(alice.last_vote_time == db.head_block_time());
            BOOST_REQUIRE(alice_comment.net_rshares.value == alice.rep_shares.amount.value * (old_voting_power - alice.voting_power) / BMCHAIN_100_PERCENT);
            BOOST_REQUIRE(alice_comment.cashout_time == alice_comment.created + BMCHAIN_CASHOUT_WINDOW_SECONDS);
            BOOST_REQUIRE(itr->rshares == alice.rep_shares.amount.value * (old_voting_power - alice.voting_power) / BMCHAIN_100_PERCENT);
            BOOST_REQUIRE(itr != vote_idx.end());
            validate_database();

            BOOST_TEST_MESSAGE("--- Test reduced power for quick voting");

            generate_blocks(db.head_block_time() + BMCHAIN_MIN_VOTE_INTERVAL_SEC);

            old_voting_power = db.get_account("alice").voting_power;

            comment_op.author = "bob";
            comment_op.permlink = "foo";
            comment_op.title = "bar";
            comment_op.body = "foo bar";
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(comment_op);
            tx.sign(bob_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);

            op.weight = BMCHAIN_100_PERCENT / 2;
            op.voter = "alice";
            op.author = "bob";
            op.permlink = "foo";
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(alice_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);

            const auto &bob_comment = db.get_comment("bob", string("foo"));
            itr = vote_idx.find(std::make_tuple(bob_comment.id, alice.id));

            BOOST_REQUIRE(db.get_account("alice").voting_power == old_voting_power -
                                                                   ((old_voting_power + max_vote_denom - 1) *
                                                                    BMCHAIN_100_PERCENT /
                                                                    (2 * max_vote_denom * BMCHAIN_100_PERCENT)));
            BOOST_REQUIRE(bob_comment.net_rshares.value == alice.rep_shares.amount.value *
                                                           (old_voting_power - db.get_account("alice").voting_power) /
                                                           BMCHAIN_100_PERCENT);
            BOOST_REQUIRE(bob_comment.cashout_time == bob_comment.created + BMCHAIN_CASHOUT_WINDOW_SECONDS);
            BOOST_REQUIRE(itr != vote_idx.end());
            validate_database();

            BOOST_TEST_MESSAGE("--- Test payout time extension on vote");

            old_voting_power = db.get_account("bob").voting_power;
            auto old_abs_rshares = db.get_comment("alice", string("foo")).abs_rshares.value;

            generate_blocks(db.head_block_time() + fc::seconds((BMCHAIN_CASHOUT_WINDOW_SECONDS / 2)), true);

            const auto &new_bob = db.get_account("bob");
            const auto &new_alice_comment = db.get_comment("alice", string("foo"));

            op.weight = BMCHAIN_100_PERCENT;
            op.voter = "bob";
            op.author = "alice";
            op.permlink = "foo";
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
            tx.sign(bob_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);

            itr = vote_idx.find(std::make_tuple(new_alice_comment.id, new_bob.id));
            uint128_t new_cashout_time = db.head_block_time().sec_since_epoch() + BMCHAIN_CASHOUT_WINDOW_SECONDS;

            BOOST_REQUIRE(new_bob.voting_power ==
                          BMCHAIN_100_PERCENT - ((BMCHAIN_100_PERCENT + max_vote_denom - 1) / max_vote_denom));
            BOOST_REQUIRE(new_alice_comment.net_rshares.value == old_abs_rshares + new_bob.rep_shares.amount.value *
                                                                                   (old_voting_power -
                                                                                    new_bob.voting_power) /
                                                                                   BMCHAIN_100_PERCENT
//                                                                 - BMCHAIN_VOTE_DUST_THRESHOLD
                                                                    );
            BOOST_REQUIRE(new_alice_comment.cashout_time == new_alice_comment.created + BMCHAIN_CASHOUT_WINDOW_SECONDS);
            BOOST_REQUIRE(itr != vote_idx.end());
            validate_database();

            BOOST_TEST_MESSAGE("--- Test negative vote");

            const auto &new_sam = db.get_account("sam");
            const auto &new_bob_comment = db.get_comment("bob", string("foo"));

            old_abs_rshares = new_bob_comment.abs_rshares.value;

            op.weight = -1 * BMCHAIN_100_PERCENT / 2;
            op.voter = "sam";
            op.author = "bob";
            op.permlink = "foo";
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(sam_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);

            itr = vote_idx.find(std::make_tuple(new_bob_comment.id, new_sam.id));
            new_cashout_time = db.head_block_time().sec_since_epoch() + BMCHAIN_CASHOUT_WINDOW_SECONDS;
            auto sam_weight /*= ( ( uint128_t( new_sam.rep_shares.amount.value ) ) / 400 + 1 ).to_uint64();*/
                    = ((uint128_t(new_sam.rep_shares.amount.value) *
                        ((BMCHAIN_100_PERCENT + max_vote_denom - 1) / (2 * max_vote_denom))) /
                       BMCHAIN_100_PERCENT).to_uint64() - BMCHAIN_VOTE_DUST_THRESHOLD;

            BOOST_REQUIRE(new_sam.voting_power ==
                          BMCHAIN_100_PERCENT - ((BMCHAIN_100_PERCENT + max_vote_denom - 1) / (2 * max_vote_denom)));
            BOOST_REQUIRE(static_cast<uint64_t>(new_bob_comment.net_rshares.value) == old_abs_rshares - sam_weight);
            BOOST_REQUIRE(static_cast<uint64_t>(new_bob_comment.abs_rshares.value) == old_abs_rshares + sam_weight);
            BOOST_REQUIRE(new_bob_comment.cashout_time == new_bob_comment.created + BMCHAIN_CASHOUT_WINDOW_SECONDS);
            BOOST_REQUIRE(itr != vote_idx.end());
            validate_database();

            BOOST_TEST_MESSAGE("--- Test nested voting on nested comments");

            old_abs_rshares = new_alice_comment.children_abs_rshares.value;
            int64_t regenerated_power = (BMCHAIN_100_PERCENT * (db.head_block_time() -
                                                              db.get_account("alice").last_vote_time).to_seconds()) /
                                        BMCHAIN_VOTE_REGENERATION_SECONDS;
            int64_t used_power =
                    (db.get_account("alice").voting_power + regenerated_power + max_vote_denom - 1) / max_vote_denom;

            comment_op.author = "sam";
            comment_op.permlink = "foo";
            comment_op.title = "bar";
            comment_op.body = "foo bar";
            comment_op.parent_author = "alice";
            comment_op.parent_permlink = "foo";
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(comment_op);
            tx.sign(sam_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);

            op.weight = BMCHAIN_100_PERCENT;
            op.voter = "alice";
            op.author = "sam";
            op.permlink = "foo";
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(alice_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);

            auto new_rshares = ((fc::uint128_t(db.get_account("alice").rep_shares.amount.value) * used_power) /
                                BMCHAIN_100_PERCENT).to_uint64() - BMCHAIN_VOTE_DUST_THRESHOLD;

            BOOST_REQUIRE(db.get_comment("alice", string("foo")).cashout_time ==
                          db.get_comment("alice", string("foo")).created + BMCHAIN_CASHOUT_WINDOW_SECONDS);

            validate_database();

            BOOST_TEST_MESSAGE("--- Test increasing vote rshares");

            generate_blocks(db.head_block_time() + BMCHAIN_MIN_VOTE_INTERVAL_SEC);

            auto new_alice = db.get_account("alice");
            auto alice_bob_vote = vote_idx.find(std::make_tuple(new_bob_comment.id, new_alice.id));
            auto old_vote_rshares = alice_bob_vote->rshares;
            auto old_net_rshares = new_bob_comment.net_rshares.value;
            old_abs_rshares = new_bob_comment.abs_rshares.value;
            used_power = ((BMCHAIN_1_PERCENT * 25 * (new_alice.voting_power) / BMCHAIN_100_PERCENT) + max_vote_denom - 1) /
                         max_vote_denom;
            auto alice_voting_power = new_alice.voting_power - used_power;

            op.voter = "alice";
            op.weight = BMCHAIN_1_PERCENT * 25;
            op.author = "bob";
            op.permlink = "foo";
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(alice_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);
            alice_bob_vote = vote_idx.find(std::make_tuple(new_bob_comment.id, new_alice.id));

            new_rshares = ((fc::uint128_t(new_alice.rep_shares.amount.value) * used_power) /
                           BMCHAIN_100_PERCENT).to_uint64() - BMCHAIN_VOTE_DUST_THRESHOLD;

            BOOST_REQUIRE(new_bob_comment.net_rshares == old_net_rshares - old_vote_rshares + new_rshares);
            BOOST_REQUIRE(new_bob_comment.abs_rshares == old_abs_rshares + new_rshares);
            BOOST_REQUIRE(new_bob_comment.cashout_time == new_bob_comment.created + BMCHAIN_CASHOUT_WINDOW_SECONDS);
            BOOST_REQUIRE(static_cast<uint64_t>(alice_bob_vote->rshares) == new_rshares);
            BOOST_REQUIRE(alice_bob_vote->last_update == db.head_block_time());
            BOOST_REQUIRE(alice_bob_vote->vote_percent == op.weight);
            BOOST_REQUIRE(db.get_account("alice").voting_power == alice_voting_power);
            validate_database();

            BOOST_TEST_MESSAGE("--- Test decreasing vote rshares");

            generate_blocks(db.head_block_time() + BMCHAIN_MIN_VOTE_INTERVAL_SEC);

            old_vote_rshares = new_rshares;
            old_net_rshares = new_bob_comment.net_rshares.value;
            old_abs_rshares = new_bob_comment.abs_rshares.value;
            used_power = (uint64_t(BMCHAIN_1_PERCENT) * 75 * uint64_t(alice_voting_power)) / BMCHAIN_100_PERCENT;
            used_power = (used_power + max_vote_denom - 1) / max_vote_denom;
            alice_voting_power -= used_power;

            op.weight = BMCHAIN_1_PERCENT * -75;
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(alice_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);
            alice_bob_vote = vote_idx.find(std::make_tuple(new_bob_comment.id, new_alice.id));

            new_rshares = ((fc::uint128_t(new_alice.rep_shares.amount.value) * used_power) /
                           BMCHAIN_100_PERCENT).to_uint64() - BMCHAIN_VOTE_DUST_THRESHOLD;

            BOOST_REQUIRE(new_bob_comment.net_rshares == old_net_rshares - old_vote_rshares - new_rshares);
            BOOST_REQUIRE(new_bob_comment.abs_rshares == old_abs_rshares + new_rshares);
            BOOST_REQUIRE(new_bob_comment.cashout_time == new_bob_comment.created + BMCHAIN_CASHOUT_WINDOW_SECONDS);
            BOOST_REQUIRE(alice_bob_vote->rshares == -1 * static_cast<int64_t>(new_rshares));
            BOOST_REQUIRE(alice_bob_vote->last_update == db.head_block_time());
            BOOST_REQUIRE(alice_bob_vote->vote_percent == op.weight);
            BOOST_REQUIRE(db.get_account("alice").voting_power == alice_voting_power);
            validate_database();

            BOOST_TEST_MESSAGE("--- Test changing a vote to 0 weight (aka: removing a vote)");

            generate_blocks(db.head_block_time() + BMCHAIN_MIN_VOTE_INTERVAL_SEC);

            old_vote_rshares = alice_bob_vote->rshares;
            old_net_rshares = new_bob_comment.net_rshares.value;
            old_abs_rshares = new_bob_comment.abs_rshares.value;

            op.weight = 0;
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(alice_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);
            alice_bob_vote = vote_idx.find(std::make_tuple(new_bob_comment.id, new_alice.id));

            BOOST_REQUIRE(new_bob_comment.net_rshares == old_net_rshares - old_vote_rshares);
            BOOST_REQUIRE(new_bob_comment.abs_rshares == old_abs_rshares);
            BOOST_REQUIRE(new_bob_comment.cashout_time == new_bob_comment.created + BMCHAIN_CASHOUT_WINDOW_SECONDS);
            BOOST_REQUIRE(alice_bob_vote->rshares == 0);
            BOOST_REQUIRE(alice_bob_vote->last_update == db.head_block_time());
            BOOST_REQUIRE(alice_bob_vote->vote_percent == op.weight);
            BOOST_REQUIRE(db.get_account("alice").voting_power == alice_voting_power);
            validate_database();

            BOOST_TEST_MESSAGE("--- Test failure when increasing rshares within lockout period");

            generate_blocks(fc::time_point_sec(
                    (new_bob_comment.cashout_time - BMCHAIN_UPVOTE_LOCKOUT).sec_since_epoch() +
                    BMCHAIN_BLOCK_INTERVAL), true);

            op.weight = BMCHAIN_100_PERCENT;
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(alice_private_key, db.get_chain_id());

            BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception);
            validate_database();

            BOOST_TEST_MESSAGE("--- Test success when reducing rshares within lockout period");

            op.weight = -1 * BMCHAIN_100_PERCENT;
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(alice_private_key, db.get_chain_id());
            db.push_transaction(tx, 0);
            validate_database();

            BOOST_TEST_MESSAGE("--- Test failure with a new vote within lockout period");

            op.weight = BMCHAIN_100_PERCENT;
            op.voter = "dave";
            tx.operations.clear();
            tx.signatures.clear();
            tx.operations.push_back(op);
            tx.sign(dave_private_key, db.get_chain_id());
            BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception);
            validate_database();
        }
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( transfer_validate ) {
   try
   {
      BOOST_TEST_MESSAGE( "Testing: transfer_validate" );

      validate_database();
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( transfer_authorities ) {
   try
   {
      ACTORS( (alice)(bob) )
      fund( "alice", 10000 );

      BOOST_TEST_MESSAGE( "Testing: transfer_authorities" );

      transfer_operation op;
      op.from = "alice";
      op.to = "bob";
      op.amount = ASSET( "2.500 TESTS" );

      signed_transaction tx;
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.operations.push_back( op );

      BOOST_TEST_MESSAGE( "--- Test failure when no signatures" );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), tx_missing_active_auth );

      BOOST_TEST_MESSAGE( "--- Test failure when signed by a signature not in the account's authority" );
      tx.sign( alice_post_key, db.get_chain_id() );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), tx_missing_active_auth );

      BOOST_TEST_MESSAGE( "--- Test failure when duplicate signatures" );
      tx.signatures.clear();
      tx.sign( alice_private_key, db.get_chain_id() );
      tx.sign( alice_private_key, db.get_chain_id() );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), tx_duplicate_sig );

      BOOST_TEST_MESSAGE( "--- Test failure when signed by an additional signature not in the creator's authority" );
      tx.signatures.clear();
      tx.sign( alice_private_key, db.get_chain_id() );
      tx.sign( bob_private_key, db.get_chain_id() );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), tx_irrelevant_sig );

      BOOST_TEST_MESSAGE( "--- Test success with witness signature" );
      tx.signatures.clear();
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      validate_database();
   }
   FC_LOG_AND_RETHROW()
}
    
BOOST_AUTO_TEST_CASE( signature_stripping )
{
    try
    {
        // Alice, Bob and Sam all have 2-of-3 multisig on corp.
        // Legitimate tx signed by (Alice, Bob) goes through.
        // Sam shouldn't be able to add or remove signatures to get the transaction to process multiple times.

        ACTORS((alice)(bob)(sam)(corp))
        fund("corp", 10000);

        account_update_operation update_op;
        update_op.account = "corp";
        update_op.active = authority(2, "alice", 1, "bob", 1, "sam", 1);

        signed_transaction tx;
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
        tx.operations.push_back(update_op);

        tx.sign(corp_private_key, db.get_chain_id());
        db.push_transaction(tx, 0);

        tx.operations.clear();
        tx.signatures.clear();

        transfer_operation transfer_op;
        transfer_op.from = "corp";
        transfer_op.to = "sam";
        transfer_op.amount = ASSET("1.000 TESTS");

        tx.operations.push_back(transfer_op);

        tx.sign(alice_private_key, db.get_chain_id());
        signature_type alice_sig = tx.signatures.back();
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_missing_active_auth);
        tx.sign(bob_private_key, db.get_chain_id());
        signature_type bob_sig = tx.signatures.back();
        tx.sign(sam_private_key, db.get_chain_id());
        signature_type sam_sig = tx.signatures.back();
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_irrelevant_sig);

        tx.signatures.clear();
        tx.signatures.push_back(alice_sig);
        tx.signatures.push_back(bob_sig);
        db.push_transaction(tx, 0);

        tx.signatures.clear();
        tx.signatures.push_back(alice_sig);
        tx.signatures.push_back(sam_sig);
        BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), fc::exception );
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( transfer_apply )
{
    try
    {
        BOOST_TEST_MESSAGE("Testing: transfer_apply");

        ACTORS((alice)(bob))
        fund("alice", 10000);

        BOOST_REQUIRE(alice.balance.amount.value == ASSET("10.000 TESTS").amount.value);
        BOOST_REQUIRE(bob.balance.amount.value == ASSET(" 0.000 TESTS").amount.value);

        signed_transaction tx;
        transfer_operation op;

        op.from = "alice";
        op.to = "bob";
        op.amount = ASSET("5.000 TESTS");

        BOOST_TEST_MESSAGE("--- Test normal transaction");
        tx.operations.push_back(op);
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
        tx.sign(alice_private_key, db.get_chain_id());
        db.push_transaction(tx, 0);

        BOOST_REQUIRE(alice.balance.amount.value == ASSET("5.000 TESTS").amount.value);
        BOOST_REQUIRE(bob.balance.amount.value == ASSET("5.000 TESTS").amount.value);
        validate_database();

        BOOST_TEST_MESSAGE("--- Generating a block");
        generate_block();

        const auto &new_alice = db.get_account("alice");
        const auto &new_bob = db.get_account("bob");

        BOOST_REQUIRE(new_alice.balance.amount.value == ASSET("5.000 TESTS").amount.value);
        BOOST_REQUIRE(new_bob.balance.amount.value == ASSET("5.000 TESTS").amount.value);
        validate_database();

        BOOST_TEST_MESSAGE("--- Test emptying an account");
        tx.signatures.clear();
        tx.operations.clear();
        tx.operations.push_back(op);
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
        tx.sign(alice_private_key, db.get_chain_id());
        db.push_transaction(tx, database::skip_transaction_dupe_check);

        BOOST_REQUIRE(new_alice.balance.amount.value == ASSET("0.000 TESTS").amount.value);
        BOOST_REQUIRE(new_bob.balance.amount.value == ASSET("10.000 TESTS").amount.value);
        validate_database();

        BOOST_TEST_MESSAGE("--- Test transferring non-existent funds");
        tx.signatures.clear();
        tx.operations.clear();
        tx.operations.push_back(op);
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
        tx.sign(alice_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, database::skip_transaction_dupe_check), fc::exception);

        BOOST_REQUIRE(new_alice.balance.amount.value == ASSET("0.000 TESTS").amount.value);
        BOOST_REQUIRE(new_bob.balance.amount.value == ASSET("10.000 TESTS").amount.value);
        validate_database();

    }
    FC_LOG_AND_RETHROW()
}
    
BOOST_AUTO_TEST_CASE( witness_update_validate )
{
   try
   {
      BOOST_TEST_MESSAGE( "Testing: withness_update_validate" );

      validate_database();
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( witness_update_authorities )
{
   try
   {
      BOOST_TEST_MESSAGE( "Testing: witness_update_authorities" );

      ACTORS( (alice)(bob) );
      fund( "alice", 10000 );

      private_key_type signing_key = generate_private_key( "new_key" );

      witness_update_operation op;
      op.owner = "alice";
      op.url = "foo.bar";
      op.fee = ASSET( "1.000 TESTS" );
      op.block_signing_key = signing_key.get_public_key();

      signed_transaction tx;
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.operations.push_back( op );

      BOOST_TEST_MESSAGE( "--- Test failure when no signatures" );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), tx_missing_active_auth );

      BOOST_TEST_MESSAGE( "--- Test failure when signed by a signature not in the account's authority" );
      tx.sign( alice_post_key, db.get_chain_id() );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), tx_missing_active_auth );

      BOOST_TEST_MESSAGE( "--- Test failure when duplicate signatures" );
      tx.signatures.clear();
      tx.sign( alice_private_key, db.get_chain_id() );
      tx.sign( alice_private_key, db.get_chain_id() );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), tx_duplicate_sig );

      BOOST_TEST_MESSAGE( "--- Test failure when signed by an additional signature not in the creator's authority" );
      tx.signatures.clear();
      tx.sign( alice_private_key, db.get_chain_id() );
      tx.sign( bob_private_key, db.get_chain_id() );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), tx_irrelevant_sig );

      BOOST_TEST_MESSAGE( "--- Test success with witness signature" );
      tx.signatures.clear();
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.signatures.clear();
      tx.sign( signing_key, db.get_chain_id() );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, database::skip_transaction_dupe_check ), tx_missing_active_auth );
      validate_database();
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( witness_update_apply )
{
    try
    {
        BOOST_TEST_MESSAGE("Testing: witness_update_apply");

        ACTORS((alice))
        fund("alice", 10000);

        private_key_type signing_key = generate_private_key("new_key");

        BOOST_TEST_MESSAGE("--- Test upgrading an account to a witness");

        witness_update_operation op;
        op.owner = "alice";
        op.url = "foo.bar";
        op.fee = ASSET("1.000 TESTS");
        op.block_signing_key = signing_key.get_public_key();
//        op.props.account_creation_fee = legacy_steem_asset::from_asset(
//                asset(BMCHAIN_MIN_ACCOUNT_CREATION_FEE + 10, BMT_SYMBOL));
        op.props.maximum_block_size = BMCHAIN_MIN_BLOCK_SIZE_LIMIT + 100;

        signed_transaction tx;
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
        tx.operations.push_back(op);
        tx.sign(alice_private_key, db.get_chain_id());

        db.push_transaction(tx, 0);

        const witness_object &alice_witness = db.get_witness("alice");

        BOOST_REQUIRE(alice_witness.owner == "alice");
        BOOST_REQUIRE(alice_witness.created == db.head_block_time());
        BOOST_REQUIRE(to_string(alice_witness.url) == op.url);
        BOOST_REQUIRE(alice_witness.signing_key == op.block_signing_key);
//        BOOST_REQUIRE(alice_witness.props.account_creation_fee == op.props.account_creation_fee.to_asset<true>());
        BOOST_REQUIRE(alice_witness.props.maximum_block_size == op.props.maximum_block_size);
        BOOST_REQUIRE(alice_witness.total_missed == 0);
        BOOST_REQUIRE(alice_witness.last_aslot == 0);
        BOOST_REQUIRE(alice_witness.last_confirmed_block_num == 0);
        BOOST_REQUIRE(alice_witness.pow_worker == 0);
        BOOST_REQUIRE(alice_witness.votes.value == 0);
        BOOST_REQUIRE(alice_witness.virtual_last_update == 0);
        BOOST_REQUIRE(alice_witness.virtual_position == 0);
        BOOST_REQUIRE(alice_witness.virtual_scheduled_time == fc::uint128_t::max_value());
        BOOST_REQUIRE(alice.balance.amount.value == ASSET("10.000 TESTS").amount.value); // No fee
        validate_database();

        BOOST_TEST_MESSAGE("--- Test updating a witness");

        tx.signatures.clear();
        tx.operations.clear();
        op.url = "bar.foo";
        tx.operations.push_back(op);
        tx.sign(alice_private_key, db.get_chain_id());

        db.push_transaction(tx, 0);

        BOOST_REQUIRE(alice_witness.owner == "alice");
        BOOST_REQUIRE(alice_witness.created == db.head_block_time());
        BOOST_REQUIRE(to_string(alice_witness.url) == "bar.foo");
        BOOST_REQUIRE(alice_witness.signing_key == op.block_signing_key);
//        BOOST_REQUIRE(alice_witness.props.account_creation_fee == op.props.account_creation_fee.to_asset<true>());
        BOOST_REQUIRE(alice_witness.props.maximum_block_size == op.props.maximum_block_size);
        BOOST_REQUIRE(alice_witness.total_missed == 0);
        BOOST_REQUIRE(alice_witness.last_aslot == 0);
        BOOST_REQUIRE(alice_witness.last_confirmed_block_num == 0);
        BOOST_REQUIRE(alice_witness.pow_worker == 0);
        BOOST_REQUIRE(alice_witness.votes.value == 0);
        BOOST_REQUIRE(alice_witness.virtual_last_update == 0);
        BOOST_REQUIRE(alice_witness.virtual_position == 0);
        BOOST_REQUIRE(alice_witness.virtual_scheduled_time == fc::uint128_t::max_value());
        BOOST_REQUIRE(alice.balance.amount.value == ASSET("10.000 TESTS").amount.value);
        validate_database();

        BOOST_TEST_MESSAGE("--- Test failure when upgrading a non-existent account");

        tx.signatures.clear();
        tx.operations.clear();
        op.owner = "bob";
        tx.operations.push_back(op);
        tx.sign(alice_private_key, db.get_chain_id());
//        STEEM_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception);
        validate_database();
    }
    FC_LOG_AND_RETHROW()
}


BOOST_AUTO_TEST_CASE( account_witness_vote_validate )
{
   try
   {
       BOOST_TEST_MESSAGE( "Testing: account_witness_vote_validate" );

       validate_database();
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( account_witness_vote_authorities )
{
    try
    {
        BOOST_TEST_MESSAGE("Testing: account_witness_vote_authorities");

        ACTORS((alice)(bob)(sam))

        fund("alice", 1000);
        private_key_type alice_witness_key = generate_private_key("alice_witness");
        witness_create("alice", alice_private_key, "foo.bar", alice_witness_key.get_public_key(), 1000);

        account_witness_vote_operation op;
        op.account = "bob";
        op.witness = "alice";

        signed_transaction tx;
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
        tx.operations.push_back(op);

        BOOST_TEST_MESSAGE("--- Test failure when no signatures");
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_missing_active_auth);

        BOOST_TEST_MESSAGE("--- Test failure when signed by a signature not in the account's authority");
        tx.sign(bob_post_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_missing_active_auth);

        BOOST_TEST_MESSAGE("--- Test failure when duplicate signatures");
        tx.signatures.clear();
        tx.sign(bob_private_key, db.get_chain_id());
        tx.sign(bob_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_duplicate_sig);

        BOOST_TEST_MESSAGE("--- Test failure when signed by an additional signature not in the creator's authority");
        tx.signatures.clear();
        tx.sign(bob_private_key, db.get_chain_id());
        tx.sign(alice_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_irrelevant_sig);

        BOOST_TEST_MESSAGE("--- Test success with witness signature");
        tx.signatures.clear();
        tx.sign(bob_private_key, db.get_chain_id());
        db.push_transaction(tx, 0);

        BOOST_TEST_MESSAGE("--- Test failure with proxy signature");
        proxy("bob", "sam");
        tx.signatures.clear();
        tx.sign(sam_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, database::skip_transaction_dupe_check), tx_missing_active_auth);

        validate_database();
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( account_witness_vote_apply )
{
    try
    {
        BOOST_TEST_MESSAGE("Testing: account_witness_vote_apply");

        ACTORS((alice)(bob)(sam))
        fund("alice", 5000);
        vest("alice", 5000);
        fund("sam", 1000);

        private_key_type sam_witness_key = generate_private_key("sam_key");
        witness_create("sam", sam_private_key, "foo.bar", sam_witness_key.get_public_key(), 1000);
        const witness_object &sam_witness = db.get_witness("sam");

        const auto &witness_vote_idx = db.get_index<witness_vote_index>().indices().get<by_witness_account>();

        BOOST_TEST_MESSAGE("--- Test normal vote");
        account_witness_vote_operation op;
        op.account = "alice";
        op.witness = "sam";
        op.approve = true;

        signed_transaction tx;
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
        tx.operations.push_back(op);
        tx.sign(alice_private_key, db.get_chain_id());

        db.push_transaction(tx, 0);

        BOOST_REQUIRE(sam_witness.votes == alice.rep_shares.amount);
        BOOST_REQUIRE(witness_vote_idx.find(std::make_tuple(sam_witness.owner, alice.name)) != witness_vote_idx.end());
        validate_database();

        BOOST_TEST_MESSAGE("--- Test revoke vote");
        op.approve = false;
        tx.operations.clear();
        tx.signatures.clear();
        tx.operations.push_back(op);
        tx.sign(alice_private_key, db.get_chain_id());

        db.push_transaction(tx, 0);
        BOOST_REQUIRE(sam_witness.votes.value == 0);
        BOOST_REQUIRE(witness_vote_idx.find(std::make_tuple(sam_witness.owner, alice.name)) == witness_vote_idx.end());

        BOOST_TEST_MESSAGE("--- Test failure when attempting to revoke a non-existent vote");

        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, database::skip_transaction_dupe_check), fc::exception);
        BOOST_REQUIRE(sam_witness.votes.value == 0);
        BOOST_REQUIRE(witness_vote_idx.find(std::make_tuple(sam_witness.owner, alice.name)) == witness_vote_idx.end());

        BOOST_TEST_MESSAGE("--- Test proxied vote");
        proxy("alice", "bob");
        tx.operations.clear();
        tx.signatures.clear();
        op.approve = true;
        op.account = "bob";
        tx.operations.push_back(op);
        tx.sign(bob_private_key, db.get_chain_id());

        db.push_transaction(tx, 0);

        BOOST_REQUIRE(sam_witness.votes == (bob.proxied_vsf_votes_total() + bob.vesting_shares.amount));
        BOOST_REQUIRE(witness_vote_idx.find(std::make_tuple(sam_witness.owner, bob.name)) != witness_vote_idx.end());
        BOOST_REQUIRE(witness_vote_idx.find(std::make_tuple(sam_witness.owner, alice.name)) == witness_vote_idx.end());

        BOOST_TEST_MESSAGE("--- Test vote from a proxied account");
        tx.operations.clear();
        tx.signatures.clear();
        op.account = "alice";
        tx.operations.push_back(op);
        tx.sign(alice_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, database::skip_transaction_dupe_check), fc::exception);

        BOOST_REQUIRE(sam_witness.votes == (bob.proxied_vsf_votes_total() + bob.vesting_shares.amount));
        BOOST_REQUIRE(witness_vote_idx.find(std::make_tuple(sam_witness.owner, bob.name)) != witness_vote_idx.end());
        BOOST_REQUIRE(witness_vote_idx.find(std::make_tuple(sam_witness.owner, alice.name)) == witness_vote_idx.end());

        BOOST_TEST_MESSAGE("--- Test revoke proxied vote");
        tx.operations.clear();
        tx.signatures.clear();
        op.account = "bob";
        op.approve = false;
        tx.operations.push_back(op);
        tx.sign(bob_private_key, db.get_chain_id());

        db.push_transaction(tx, 0);

        BOOST_REQUIRE(sam_witness.votes.value == 0);
        BOOST_REQUIRE(witness_vote_idx.find(std::make_tuple(sam_witness.owner, bob.name)) == witness_vote_idx.end());
        BOOST_REQUIRE(witness_vote_idx.find(std::make_tuple(sam_witness.owner, alice.name)) == witness_vote_idx.end());

        BOOST_TEST_MESSAGE("--- Test failure when voting for a non-existent account");
        tx.operations.clear();
        tx.signatures.clear();
        op.witness = "dave";
        op.approve = true;
        tx.operations.push_back(op);
        tx.sign(bob_private_key, db.get_chain_id());

        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception);
        validate_database();

        BOOST_TEST_MESSAGE("--- Test failure when voting for an account that is not a witness");
        tx.operations.clear();
        tx.signatures.clear();
        op.witness = "alice";
        tx.operations.push_back(op);
        tx.sign(bob_private_key, db.get_chain_id());

        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), fc::exception);
        validate_database();
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( account_witness_proxy_validate )
{
    try
    {
        BOOST_TEST_MESSAGE("Testing: account_witness_proxy_validate");

        validate_database();
    }
    FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( account_witness_proxy_authorities )
{
    try
    {
        BOOST_TEST_MESSAGE("Testing: account_witness_proxy_authorities");

        ACTORS((alice)(bob))

        account_witness_proxy_operation op;
        op.account = "bob";
        op.proxy = "alice";

        signed_transaction tx;
        tx.set_expiration(db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION);
        tx.operations.push_back(op);

        BOOST_TEST_MESSAGE("--- Test failure when no signatures");
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_missing_active_auth);

        BOOST_TEST_MESSAGE("--- Test failure when signed by a signature not in the account's authority");
        tx.sign(bob_post_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_missing_active_auth);

        BOOST_TEST_MESSAGE("--- Test failure when duplicate signatures");
        tx.signatures.clear();
        tx.sign(bob_private_key, db.get_chain_id());
        tx.sign(bob_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_duplicate_sig);

        BOOST_TEST_MESSAGE("--- Test failure when signed by an additional signature not in the creator's authority");
        tx.signatures.clear();
        tx.sign(bob_private_key, db.get_chain_id());
        tx.sign(alice_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, 0), tx_irrelevant_sig);

        BOOST_TEST_MESSAGE("--- Test success with witness signature");
        tx.signatures.clear();
        tx.sign(bob_private_key, db.get_chain_id());
        db.push_transaction(tx, 0);

        BOOST_TEST_MESSAGE("--- Test failure with proxy signature");
        tx.signatures.clear();
        tx.sign(alice_private_key, db.get_chain_id());
        BMCHAIN_REQUIRE_THROW(db.push_transaction(tx, database::skip_transaction_dupe_check), tx_missing_active_auth);

        validate_database();
    }
    FC_LOG_AND_RETHROW()
}    
    
BOOST_AUTO_TEST_CASE( encrypted_content )
{
    try {
        BOOST_TEST_MESSAGE("Testing: encrypted_content");

        ACTORS((alice)(bob))
        fund("alice", 10000);

        private_key_type signing_key = generate_private_key("new_key");

    }
    FC_LOG_AND_RETHROW()
}
    
BOOST_AUTO_TEST_SUITE_END()
#endif
