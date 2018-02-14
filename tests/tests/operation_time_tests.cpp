#ifdef IS_TEST_NET
#include <boost/test/unit_test.hpp>

#include <bmchain/protocol/exceptions.hpp>

#include <bmchain/chain/block_summary_object.hpp>
#include <bmchain/chain/database.hpp>
#include <bmchain/chain/hardfork.hpp>
#include <bmchain/chain/history_object.hpp>
#include <bmchain/chain/bmchain_objects.hpp>

#include <bmchain/chain/util/reward.hpp>

#include <bmchain/plugins/debug_node/debug_node_plugin.hpp>

#include <fc/crypto/digest.hpp>

#include "../common/database_fixture.hpp"

#include <cmath>

using namespace bmchain;
using namespace bmchain::chain;
using namespace bmchain::protocol;

BOOST_FIXTURE_TEST_SUITE( operation_time_tests, clean_database_fixture )

BOOST_AUTO_TEST_CASE( comment_payout_equalize )
{
   try
   {
      ACTORS( (alice)(bob)(dave)
              (ulysses)(vivian)(wendy) )

      struct author_actor
      {
         author_actor(
            const std::string& n,
            fc::ecc::private_key pk,
            fc::optional<asset> mpay = fc::optional<asset>() )
            : name(n), private_key(pk), max_accepted_payout(mpay) {}
         std::string             name;
         fc::ecc::private_key    private_key;
         fc::optional< asset >   max_accepted_payout;
      };

      struct voter_actor
      {
         voter_actor( const std::string& n, fc::ecc::private_key pk, std::string fa )
            : name(n), private_key(pk), favorite_author(fa) {}
         std::string             name;
         fc::ecc::private_key    private_key;
         std::string             favorite_author;
      };


      std::vector< author_actor > authors;
      std::vector< voter_actor > voters;

      authors.emplace_back( "alice", alice_private_key );
      authors.emplace_back( "bob"  , bob_private_key, ASSET( "0.000 TBD" ) );
      authors.emplace_back( "dave" , dave_private_key );
      voters.emplace_back( "ulysses", ulysses_private_key, "alice");
      voters.emplace_back( "vivian" , vivian_private_key , "bob"  );
      voters.emplace_back( "wendy"  , wendy_private_key  , "dave" );

      // A,B,D : posters
      // U,V,W : voters

      // set a ridiculously high BMT price ($1 / satoshi) to disable dust threshold
      //set_price_feed( price( ASSET( "0.001 TESTS" ), ASSET( "1.000 TBD" ) ) );

      for( const auto& voter : voters )
      {
         fund( voter.name, 10000 );
         vest( voter.name, 10000 );
      }

      // authors all write in the same block, but Bob declines payout
      for( const auto& author : authors )
      {
         signed_transaction tx;
         comment_operation com;
         com.author = author.name;
         com.permlink = "mypost";
         com.parent_author = BMCHAIN_ROOT_POST_PARENT;
         com.parent_permlink = "test";
         com.title = "Hello from "+author.name;
         com.body = "Hello, my name is "+author.name;
         tx.operations.push_back( com );

         if( author.max_accepted_payout.valid() )
         {
            comment_options_operation copt;
            copt.author = com.author;
            copt.permlink = com.permlink;
            copt.max_accepted_payout = *(author.max_accepted_payout);
            tx.operations.push_back( copt );
         }

         tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
         tx.sign( author.private_key, db.get_chain_id() );
         db.push_transaction( tx, 0 );
      }

      generate_blocks(1);

      // voters all vote in the same block with the same stake
      for( const auto& voter : voters )
      {
         signed_transaction tx;
         vote_operation vote;
         vote.voter = voter.name;
         vote.author = voter.favorite_author;
         vote.permlink = "mypost";
         vote.weight = BMCHAIN_100_PERCENT;
         tx.operations.push_back( vote );
         tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
         tx.sign( voter.private_key, db.get_chain_id() );
         db.push_transaction( tx, 0 );
      }

      auto reward_bmt = db.get_dynamic_global_properties().total_reward_fund_bmt;

      // generate a few blocks to seed the reward fund
      generate_blocks(10);
      //const auto& rf = db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME );
      //idump( (rf) );

      generate_blocks( db.get_comment( "alice", string( "mypost" ) ).cashout_time, true );
      /*
      for( const auto& author : authors )
      {
         const account_object& a = db.get_account(author.name);
         ilog( "${n} : ${steem}", ("n", author.name)("steem", a.reward_bmt_balance) );
      }
      for( const auto& voter : voters )
      {
         const account_object& a = db.get_account(voter.name);
         ilog( "${n} : ${steem}", ("n", voter.name)("steem", a.reward_bmt_balance) );
      }
      */

      const account_object& alice_account = db.get_account("alice");
      const account_object& bob_account   = db.get_account("bob");
      const account_object& dave_account  = db.get_account("dave");
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( comment_payout_dust )
{
   try
   {
      BOOST_TEST_MESSAGE( "Testing: comment_payout_dust" );

      ACTORS( (alice)(bob) )
      generate_block();

      vest( "alice", ASSET( "10.000 TESTS" ) );
      vest( "bob", ASSET( "10.000 TESTS" ) );

      //set_price_feed( price( ASSET( "1.000 TESTS" ), ASSET( "1.000 TBD" ) ) );

      generate_block();
      validate_database();

      comment_operation comment;
      comment.author = "alice";
      comment.permlink = "test";
      comment.parent_permlink = "test";
      comment.title = "test";
      comment.body = "test";
      vote_operation vote;
      vote.voter = "alice";
      vote.author = "alice";
      vote.permlink = "test";
      vote.weight = 81 * BMCHAIN_1_PERCENT;

      signed_transaction tx;
      tx.operations.push_back( comment );
      tx.operations.push_back( vote );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );
      validate_database();

      comment.author = "bob";
      vote.voter = "bob";
      vote.author = "bob";
      vote.weight = 59 * BMCHAIN_1_PERCENT;

      tx.clear();
      tx.operations.push_back( comment );
      tx.operations.push_back( vote );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );
      validate_database();

      generate_blocks( db.get_comment( "alice", string( "test" ) ).cashout_time );

      // If comments are paid out independent of order, then the last satoshi of BMT cannot be divided among them
      const auto rf = db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME );
      BOOST_REQUIRE( rf.reward_balance == ASSET( "0.001 TESTS" ) );

      validate_database();

      BOOST_TEST_MESSAGE( "Done" );
   }
   FC_LOG_AND_RETHROW()
}

/*
BOOST_AUTO_TEST_CASE( reward_funds )
{
   try
   {
      BOOST_TEST_MESSAGE( "Testing: reward_funds" );

      ACTORS( (alice)(bob) )
      generate_block();

      //set_price_feed( price( ASSET( "1.000 TESTS" ), ASSET( "1.000 TBD" ) ) );
      generate_block();

      comment_operation comment;
      vote_operation vote;
      signed_transaction tx;

      comment.author = "alice";
      comment.permlink = "test";
      comment.parent_permlink = "test";
      comment.title = "foo";
      comment.body = "bar";
      vote.voter = "alice";
      vote.author = "alice";
      vote.permlink = "test";
      vote.weight = BMCHAIN_100_PERCENT;
      tx.operations.push_back( comment );
      tx.operations.push_back( vote );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      generate_blocks( 5 );

      comment.author = "bob";
      comment.parent_author = "alice";
      vote.voter = "bob";
      vote.author = "bob";
      tx.clear();
      tx.operations.push_back( comment );
      tx.operations.push_back( vote );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      generate_blocks( db.get_comment( "alice", string( "test" ) ).cashout_time );

      {
         const auto& post_rf = db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME );
         const auto& comment_rf = db.get< reward_fund_object, by_name >( BMCHAIN_COMMENT_REWARD_FUND_NAME );

         BOOST_REQUIRE( post_rf.reward_balance.amount == 0 );
         BOOST_REQUIRE( comment_rf.reward_balance.amount > 0 );
         validate_database();
      }

      generate_blocks( db.get_comment( "bob", string( "test" ) ).cashout_time );

      {
         const auto& post_rf = db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME );
         const auto& comment_rf = db.get< reward_fund_object, by_name >( BMCHAIN_COMMENT_REWARD_FUND_NAME );

         BOOST_REQUIRE( post_rf.reward_balance.amount > 0 );
         BOOST_REQUIRE( comment_rf.reward_balance.amount == 0 );
         validate_database();
      }
   }
   FC_LOG_AND_RETHROW()
}
*/

BOOST_AUTO_TEST_CASE( recent_claims_decay )
{
   try
   {
      BOOST_TEST_MESSAGE( "Testing: recent_rshares_2decay" );
      ACTORS( (alice)(bob) )
      generate_block();

      //set_price_feed( price( ASSET( "1.000 TESTS" ), ASSET( "1.000 TBD" ) ) );
      generate_block();

      comment_operation comment;
      vote_operation vote;
      signed_transaction tx;

      comment.author = "alice";
      comment.permlink = "test";
      comment.parent_permlink = "test";
      comment.title = "foo";
      comment.body = "bar";
      vote.voter = "alice";
      vote.author = "alice";
      vote.permlink = "test";
      vote.weight = BMCHAIN_100_PERCENT;
      tx.operations.push_back( comment );
      tx.operations.push_back( vote );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      auto alice_vshares = util::evaluate_reward_curve( db.get_comment( "alice", string( "test" ) ).net_rshares.value,
         db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME ).author_reward_curve,
         db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME ).content_constant );

      generate_blocks( 5 );

      comment.author = "bob";
      vote.voter = "bob";
      vote.author = "bob";
      tx.clear();
      tx.operations.push_back( comment );
      tx.operations.push_back( vote );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      generate_blocks( db.get_comment( "alice", string( "test" ) ).cashout_time );

      {
         const auto& post_rf = db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME );

         BOOST_REQUIRE( post_rf.recent_claims == alice_vshares );
         validate_database();
      }

      auto bob_cashout_time = db.get_comment( "bob", string( "test" ) ).cashout_time;
      auto bob_vshares = util::evaluate_reward_curve( db.get_comment( "bob", string( "test" ) ).net_rshares.value,
         db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME ).author_reward_curve,
         db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME ).content_constant );

      generate_block();

      while( db.head_block_time() < bob_cashout_time )
      {
         alice_vshares -= ( alice_vshares * BMCHAIN_BLOCK_INTERVAL ) / BMCHAIN_RECENT_RSHARES_DECAY_RATE.to_seconds();
         const auto& post_rf = db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME );

         BOOST_REQUIRE( post_rf.recent_claims == alice_vshares );

         generate_block();

      }

      {
         alice_vshares -= ( alice_vshares * BMCHAIN_BLOCK_INTERVAL ) / BMCHAIN_RECENT_RSHARES_DECAY_RATE.to_seconds();
         const auto& post_rf = db.get< reward_fund_object, by_name >( BMCHAIN_POST_REWARD_FUND_NAME );

         BOOST_REQUIRE( post_rf.recent_claims == alice_vshares + bob_vshares );
         validate_database();
      }
   }
   FC_LOG_AND_RETHROW()
}

/*BOOST_AUTO_TEST_CASE( comment_payout )
{
   try
   {
      ACTORS( (alice)(bob)(sam)(dave) )
      fund( "alice", 10000 );
      vest( "alice", 10000 );
      fund( "bob", 7500 );
      vest( "bob", 7500 );
      fund( "sam", 8000 );
      vest( "sam", 8000 );
      fund( "dave", 5000 );
      vest( "dave", 5000 );

      price exchange_rate( ASSET( "1.000 TESTS" ), ASSET( "1.000 TBD" ) );
      //set_price_feed( exchange_rate );

      signed_transaction tx;

      BOOST_TEST_MESSAGE( "Creating comments." );

      comment_operation com;
      com.author = "alice";
      com.permlink = "test";
      com.parent_author = "";
      com.parent_permlink = "test";
      com.title = "foo";
      com.body = "bar";
      tx.operations.push_back( com );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();

      com.author = "bob";
      tx.operations.push_back( com );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();

      BOOST_TEST_MESSAGE( "Voting for comments." );

      vote_operation vote;
      vote.voter = "alice";
      vote.author = "alice";
      vote.permlink = "test";
      vote.weight = BMCHAIN_100_PERCENT;
      tx.operations.push_back( vote );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();

      vote.voter = "sam";
      vote.author = "alice";
      tx.operations.push_back( vote );
      tx.sign( sam_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();

      vote.voter = "bob";
      vote.author = "bob";
      tx.operations.push_back( vote );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();

      vote.voter = "dave";
      vote.author = "bob";
      tx.operations.push_back( vote );
      tx.sign( dave_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_TEST_MESSAGE( "Generate blocks up until first payout" );

      //generate_blocks( db.get_comment( "bob", string( "test" ) ).cashout_time - BMCHAIN_BLOCK_INTERVAL, true );

      auto reward_bmt = db.get_dynamic_global_properties().total_reward_fund_bmt + ASSET( "1.667 TESTS" );
      auto total_rshares2 = db.get_dynamic_global_properties().total_reward_shares2;
      auto bob_comment_rshares = db.get_comment( "bob", string( "test" ) ).net_rshares;
      auto bob_vest_shares = db.get_account( "bob" ).rep_shares;

      auto bob_comment_payout = asset( ( ( uint128_t( bob_comment_rshares.value ) * bob_comment_rshares.value * reward_bmt.amount.value ) / total_rshares2 ).to_uint64(), BMT_SYMBOL );
      auto bob_comment_discussion_rewards = asset( bob_comment_payout.amount / 4, BMT_SYMBOL );
      bob_comment_payout -= bob_comment_discussion_rewards;
      auto bob_comment_rep_reward = ( bob_comment_payout - asset( bob_comment_payout.amount / 2, BMT_SYMBOL) ) * db.get_dynamic_global_properties().get_rep_share_price();

      BOOST_TEST_MESSAGE( "Cause first payout" );

      generate_block();

      BOOST_REQUIRE( db.get_dynamic_global_properties().total_reward_fund_bmt == reward_bmt - bob_comment_payout );
      BOOST_REQUIRE( db.get_comment( "bob", string( "test" ) ).total_payout_value == bob_comment_rep_reward * db.get_dynamic_global_properties().get_rep_share_price());
      BOOST_REQUIRE( db.get_account( "bob" ).rep_shares == bob_vest_shares + bob_comment_rep_reward );

      BOOST_TEST_MESSAGE( "Testing no payout when less than $0.02" );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "alice";
      vote.author = "alice";
      tx.operations.push_back( vote );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "dave";
      vote.author = "bob";
      vote.weight = BMCHAIN_1_PERCENT;
      tx.operations.push_back( vote );
      tx.sign( dave_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      generate_blocks( db.get_comment( "bob", string( "test" ) ).cashout_time - BMCHAIN_BLOCK_INTERVAL, true );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "bob";
      vote.author = "alice";
      vote.weight = BMCHAIN_100_PERCENT;
      tx.operations.push_back( vote );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "sam";
      tx.operations.push_back( vote );
      tx.sign( sam_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "dave";
      tx.operations.push_back( vote );
      tx.sign( dave_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      bob_vest_shares = db.get_account( "bob" ).rep_shares;

      validate_database();

      generate_block();

      BOOST_REQUIRE( bob_vest_shares.amount.value == db.get_account( "bob" ).rep_shares.amount.value );
      validate_database();
   }
   FC_LOG_AND_RETHROW()
}*/

/*
BOOST_AUTO_TEST_CASE( comment_payout )
{
   try
   {
      ACTORS( (alice)(bob)(sam)(dave) )
      fund( "alice", 10000 );
      vest( "alice", 10000 );
      fund( "bob", 7500 );
      vest( "bob", 7500 );
      fund( "sam", 8000 );
      vest( "sam", 8000 );
      fund( "dave", 5000 );
      vest( "dave", 5000 );

      price exchange_rate( ASSET( "1.000 TESTS" ), ASSET( "1.000 TBD" ) );
      //set_price_feed( exchange_rate );

      auto gpo = db.get_dynamic_global_properties();

      signed_transaction tx;

      BOOST_TEST_MESSAGE( "Creating comments. " );

      comment_operation com;
      com.author = "alice";
      com.permlink = "test";
      com.parent_permlink = "test";
      com.title = "foo";
      com.body = "bar";
      tx.operations.push_back( com );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      com.author = "bob";
      tx.operations.push_back( com );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_TEST_MESSAGE( "First round of votes." );

      tx.operations.clear();
      tx.signatures.clear();
      vote_operation vote;
      vote.voter = "alice";
      vote.author = "alice";
      vote.permlink = "test";
      vote.weight = BMCHAIN_100_PERCENT;
      tx.operations.push_back( vote );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "dave";
      tx.operations.push_back( vote );
      tx.sign( dave_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "bob";
      vote.author = "bob";
      tx.operations.push_back( vote );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "sam";
      tx.operations.push_back( vote );
      tx.sign( sam_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_TEST_MESSAGE( "Generating blocks..." );

      generate_blocks( fc::time_point_sec( db.head_block_time().sec_since_epoch() + BMCHAIN_CASHOUT_WINDOW_SECONDS / 2 ), true );

      BOOST_TEST_MESSAGE( "Second round of votes." );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "alice";
      vote.author = "bob";
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.operations.push_back( vote );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "bob";
      vote.author = "alice";
      tx.operations.push_back( vote );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "sam";
      tx.operations.push_back( vote );
      tx.sign( sam_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_TEST_MESSAGE( "Generating more blocks..." );

      generate_blocks( db.get_comment( "bob", string( "test" ) ).cashout_time - ( BMCHAIN_BLOCK_INTERVAL / 2 ), true );

      BOOST_TEST_MESSAGE( "Check comments have not been paid out." );

      const auto& vote_idx = db.get_index< comment_vote_index >().indices().get< by_comment_voter >();

      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "alice" ) ).id ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "bob" ) ).id   ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "sam" ) ).id   ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "dave" ) ).id  ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "alice" ) ).id ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "bob" ) ).id   ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "sam" ) ).id   ) ) != vote_idx.end() );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).net_rshares.value > 0 );
      BOOST_REQUIRE( db.get_comment( "bob", string( "test" ) ).net_rshares.value > 0 );
      validate_database();

      auto reward_bmt = db.get_dynamic_global_properties().total_reward_fund_bmt + ASSET( "2.000 TESTS" );
      auto total_rshares2 = db.get_dynamic_global_properties().total_reward_shares2;
      auto bob_comment_vote_total = db.get_comment( "bob", string( "test" ) ).total_vote_weight;
      auto bob_comment_rshares = db.get_comment( "bob", string( "test" ) ).net_rshares;
      auto alice_vest_shares = db.get_account( "alice" ).rep_shares;
      auto bob_vest_shares = db.get_account( "bob" ).rep_shares;
      auto sam_vest_shares = db.get_account( "sam" ).rep_shares;
      auto dave_vest_shares = db.get_account( "dave" ).rep_shares;

      auto bob_comment_payout = asset( ( ( uint128_t( bob_comment_rshares.value ) * bob_comment_rshares.value * reward_bmt.amount.value ) / total_rshares2 ).to_uint64(), BMT_SYMBOL );
      auto bob_comment_vote_rewards = asset( bob_comment_payout.amount / 2, BMT_SYMBOL );
      bob_comment_payout -= bob_comment_vote_rewards;
      auto bob_comment_rep_reward = ( bob_comment_payout - asset( bob_comment_payout.amount / 2, BMT_SYMBOL ) ) * db.get_dynamic_global_properties().get_rep_share_price();
      auto unclaimed_payments = bob_comment_vote_rewards;
      auto alice_vote_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id, db.get_account( "alice" ) ).id ) )->weight ) * bob_comment_vote_rewards.amount.value ) / bob_comment_vote_total ), BMT_SYMBOL );
      auto alice_vote_rep = alice_vote_reward * db.get_dynamic_global_properties().get_rep_share_price();
      auto bob_vote_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id, db.get_account( "bob" ) ).id ) )->weight ) * bob_comment_vote_rewards.amount.value ) / bob_comment_vote_total ), BMT_SYMBOL );
      auto bob_vote_rep = bob_vote_reward * db.get_dynamic_global_properties().get_rep_share_price();
      auto sam_vote_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id, db.get_account( "sam" ) ).id ) )->weight ) * bob_comment_vote_rewards.amount.value ) / bob_comment_vote_total ), BMT_SYMBOL );
      auto sam_vote_rep = sam_vote_reward * db.get_dynamic_global_properties().get_rep_share_price();
      unclaimed_payments -= ( alice_vote_reward + bob_vote_reward + sam_vote_reward );

      BOOST_TEST_MESSAGE( "Generate one block to cause a payout" );

      generate_block();

      auto bob_comment_reward = get_last_operations( 1 )[0].get< comment_reward_operation >();

      BOOST_REQUIRE( db.get_dynamic_global_properties().total_reward_fund_bmt.amount.value == reward_bmt.amount.value - ( bob_comment_payout + bob_comment_vote_rewards - unclaimed_payments ).amount.value );
      BOOST_REQUIRE( db.get_comment( "bob", string( "test" ) ).total_payout_value.amount.value == ( ( bob_comment_rep_reward * db.get_dynamic_global_properties().get_rep_share_price() )).amount.value );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).net_rshares.value > 0 );
      BOOST_REQUIRE( db.get_comment( "bob", string( "test" ) ).net_rshares.value == 0 );
      BOOST_REQUIRE( db.get_account( "alice" ).rep_shares.amount.value == ( alice_vest_shares + alice_vote_rep ).amount.value );
      BOOST_REQUIRE( db.get_account( "bob" ).rep_shares.amount.value == ( bob_vest_shares + bob_vote_rep + bob_comment_rep_reward ).amount.value );
      BOOST_REQUIRE( db.get_account( "sam" ).rep_shares.amount.value == ( sam_vest_shares + sam_vote_rep ).amount.value );
      BOOST_REQUIRE( db.get_account( "dave" ).rep_shares.amount.value == dave_vest_shares.amount.value );
      BOOST_REQUIRE( bob_comment_reward.author == "bob" );
      BOOST_REQUIRE( bob_comment_reward.permlink == "test" );
      BOOST_REQUIRE( bob_comment_reward.rep_payout.amount.value == bob_comment_rep_reward.amount.value );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "alice" ) ).id ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "bob" ) ).id   ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "sam" ) ).id   ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "dave" ) ).id  ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "alice" ) ).id ) ) == vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "bob" ) ).id   ) ) == vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "sam" ) ).id   ) ) == vote_idx.end() );
      validate_database();

      BOOST_TEST_MESSAGE( "Generating blocks up to next comment payout" );

      generate_blocks( db.get_comment( "alice", string( "test" ) ).cashout_time - ( BMCHAIN_BLOCK_INTERVAL / 2 ), true );

      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "alice" ) ).id ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "bob" ) ).id   ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "sam" ) ).id   ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "dave" ) ).id  ) ) != vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "alice" ) ).id ) ) == vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "bob" ) ).id   ) ) == vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "sam" ) ).id   ) ) == vote_idx.end() );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).net_rshares.value > 0 );
      BOOST_REQUIRE( db.get_comment( "bob", string( "test" ) ).net_rshares.value == 0 );
      validate_database();

      BOOST_TEST_MESSAGE( "Generate block to cause payout" );

      reward_bmt = db.get_dynamic_global_properties().total_reward_fund_bmt + ASSET( "2.000 TESTS" );
      total_rshares2 = db.get_dynamic_global_properties().total_reward_shares2;
      auto alice_comment_vote_total = db.get_comment( "alice", string( "test" ) ).total_vote_weight;
      auto alice_comment_rshares = db.get_comment( "alice", string( "test" ) ).net_rshares;
      alice_vest_shares = db.get_account( "alice" ).rep_shares;
      bob_vest_shares = db.get_account( "bob" ).rep_shares;
      sam_vest_shares = db.get_account( "sam" ).rep_shares;
      dave_vest_shares = db.get_account( "dave" ).rep_shares;

      u256 rs( alice_comment_rshares.value );
      u256 rf( reward_bmt.amount.value );
      u256 trs2 = total_rshares2.hi;
      trs2 = ( trs2 << 64 ) + total_rshares2.lo;
      auto rs2 = rs*rs;

      auto alice_comment_payout = asset( static_cast< uint64_t >( ( rf * rs2 ) / trs2 ), BMT_SYMBOL );
      auto alice_comment_vote_rewards = asset( alice_comment_payout.amount / 2, BMT_SYMBOL );
      alice_comment_payout -= alice_comment_vote_rewards;
      auto alice_comment_rep_reward = ( alice_comment_payout - asset( alice_comment_payout.amount / 2, BMT_SYMBOL ) ) * db.get_dynamic_global_properties().get_rep_share_price();
      unclaimed_payments = alice_comment_vote_rewards;
      alice_vote_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "alice" ) ).id ) )->weight ) * alice_comment_vote_rewards.amount.value ) / alice_comment_vote_total ), BMT_SYMBOL );
      alice_vote_rep = alice_vote_reward * db.get_dynamic_global_properties().get_rep_share_price();
      bob_vote_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "bob" ) ).id ) )->weight ) * alice_comment_vote_rewards.amount.value ) / alice_comment_vote_total ), BMT_SYMBOL );
      bob_vote_rep = bob_vote_reward * db.get_dynamic_global_properties().get_rep_share_price();
      sam_vote_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "sam" ) ).id ) )->weight ) * alice_comment_vote_rewards.amount.value ) / alice_comment_vote_total ), BMT_SYMBOL );
      sam_vote_rep = sam_vote_reward * db.get_dynamic_global_properties().get_rep_share_price();
      auto dave_vote_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "dave" ) ).id ) )->weight ) * alice_comment_vote_rewards.amount.value ) / alice_comment_vote_total ), BMT_SYMBOL );
      auto dave_vote_rep = dave_vote_reward * db.get_dynamic_global_properties().get_rep_share_price();
      unclaimed_payments -= ( alice_vote_reward + bob_vote_reward + sam_vote_reward + dave_vote_reward );

      generate_block();
      auto alice_comment_reward = get_last_operations( 1 )[0].get< comment_reward_operation >();

      BOOST_REQUIRE( ( db.get_dynamic_global_properties().total_reward_fund_bmt + alice_comment_payout + alice_comment_vote_rewards - unclaimed_payments ).amount.value == reward_bmt.amount.value );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).total_payout_value.amount.value == ( ( alice_comment_rep_reward * db.get_dynamic_global_properties().get_rep_share_price() )).amount.value );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).net_rshares.value == 0 );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).net_rshares.value == 0 );
      BOOST_REQUIRE( db.get_account( "alice" ).rep_shares.amount.value == ( alice_vest_shares + alice_vote_rep + alice_comment_rep_reward ).amount.value );
      BOOST_REQUIRE( db.get_account( "bob" ).rep_shares.amount.value == ( bob_vest_shares + bob_vote_rep ).amount.value );
      BOOST_REQUIRE( db.get_account( "sam" ).rep_shares.amount.value == ( sam_vest_shares + sam_vote_rep ).amount.value );
      BOOST_REQUIRE( db.get_account( "dave" ).rep_shares.amount.value == ( dave_vest_shares + dave_vote_rep ).amount.value );
      BOOST_REQUIRE( alice_comment_reward.author == "alice" );
      BOOST_REQUIRE( alice_comment_reward.permlink == "test" );
      BOOST_REQUIRE( alice_comment_reward.rep_payout.amount.value == alice_comment_rep_reward.amount.value );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "alice" ) ).id ) ) == vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "bob" ) ).id   ) ) == vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "sam" ) ).id   ) ) == vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "dave" ) ).id  ) ) == vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "alice" ) ).id ) ) == vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "bob" ) ).id   ) ) == vote_idx.end() );
      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id,   db.get_account( "sam" ) ).id   ) ) == vote_idx.end() );
      validate_database();

      BOOST_TEST_MESSAGE( "Testing no payout when less than $0.02" );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "alice";
      vote.author = "alice";
      tx.operations.push_back( vote );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "dave";
      vote.author = "bob";
      vote.weight = BMCHAIN_1_PERCENT;
      tx.operations.push_back( vote );
      tx.sign( dave_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      generate_blocks( db.get_comment( "bob", string( "test" ) ).cashout_time - BMCHAIN_BLOCK_INTERVAL, true );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "bob";
      vote.author = "alice";
      vote.weight = BMCHAIN_100_PERCENT;
      tx.operations.push_back( vote );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "sam";
      tx.operations.push_back( vote );
      tx.sign( sam_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote.voter = "dave";
      tx.operations.push_back( vote );
      tx.sign( dave_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      bob_vest_shares = db.get_account( "bob" ).rep_shares;

      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id, db.get_account( "dave" ) ).id ) ) != vote_idx.end() );
      validate_database();

      generate_block();

      BOOST_REQUIRE( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id, db.get_account( "dave" ) ).id ) ) == vote_idx.end() );
      BOOST_REQUIRE( bob_vest_shares.amount.value == db.get_account( "bob" ).rep_shares.amount.value );
      validate_database();
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( nested_comments )
{
   try
   {
      ACTORS( (alice)(bob)(sam)(dave) )
      fund( "alice", 10000 );
      vest( "alice", 10000 );
      fund( "bob", 10000 );
      vest( "bob", 10000 );
      fund( "sam", 10000 );
      vest( "sam", 10000 );
      fund( "dave", 10000 );
      vest( "dave", 10000 );

      price exchange_rate( ASSET( "1.000 TESTS" ), ASSET( "1.000 TBD" ) );
      //set_price_feed( exchange_rate );

      signed_transaction tx;
      comment_operation comment_op;
      comment_op.author = "alice";
      comment_op.permlink = "test";
      comment_op.parent_permlink = "test";
      comment_op.title = "foo";
      comment_op.body = "bar";
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.operations.push_back( comment_op );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      comment_op.author = "bob";
      comment_op.parent_author = "alice";
      comment_op.parent_permlink = "test";
      tx.operations.clear();
      tx.signatures.clear();
      tx.operations.push_back( comment_op );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      comment_op.author = "sam";
      comment_op.parent_author = "bob";
      tx.operations.clear();
      tx.signatures.clear();
      tx.operations.push_back( comment_op );
      tx.sign( sam_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      comment_op.author = "dave";
      comment_op.parent_author = "sam";
      tx.operations.clear();
      tx.signatures.clear();
      tx.operations.push_back( comment_op );
      tx.sign( dave_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();

      vote_operation vote_op;
      vote_op.voter = "alice";
      vote_op.author = "alice";
      vote_op.permlink = "test";
      vote_op.weight = BMCHAIN_100_PERCENT;
      tx.operations.push_back( vote_op );
      vote_op.author = "bob";
      tx.operations.push_back( vote_op );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote_op.voter = "bob";
      vote_op.author = "alice";
      tx.operations.push_back( vote_op );
      vote_op.author = "bob";
      tx.operations.push_back( vote_op );
      vote_op.author = "dave";
      vote_op.weight = BMCHAIN_1_PERCENT * 20;
      tx.operations.push_back( vote_op );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();
      vote_op.voter = "sam";
      vote_op.author = "bob";
      vote_op.weight = BMCHAIN_100_PERCENT;
      tx.operations.push_back( vote_op );
      tx.sign( sam_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      generate_blocks( db.get_comment( "alice", string( "test" ) ).cashout_time - fc::seconds( BMCHAIN_BLOCK_INTERVAL ), true );

      auto gpo = db.get_dynamic_global_properties();
      uint128_t reward_bmt = gpo.total_reward_fund_bmt.amount.value + ASSET( "2.000 TESTS" ).amount.value;
      uint128_t total_rshares2 = gpo.total_reward_shares2;

      auto alice_comment = db.get_comment( "alice", string( "test" ) );
      auto bob_comment = db.get_comment( "bob", string( "test" ) );
      auto sam_comment = db.get_comment( "sam", string( "test" ) );
      auto dave_comment = db.get_comment( "dave", string( "test" ) );

      const auto& vote_idx = db.get_index< comment_vote_index >().indices().get< by_comment_voter >();

      // Calculate total comment rewards and voting rewards.
      auto alice_comment_reward = ( ( reward_bmt * alice_comment.net_rshares.value * alice_comment.net_rshares.value ) / total_rshares2 ).to_uint64();
      total_rshares2 -= uint128_t( alice_comment.net_rshares.value ) * ( alice_comment.net_rshares.value );
      reward_bmt -= alice_comment_reward;
      auto alice_comment_vote_rewards = alice_comment_reward / 2;
      alice_comment_reward -= alice_comment_vote_rewards;

      auto alice_vote_alice_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "alice" ) ).id ) )->weight ) * alice_comment_vote_rewards ) / alice_comment.total_vote_weight ), BMT_SYMBOL );
      auto bob_vote_alice_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "alice", string( "test" ).id, db.get_account( "bob" ) ).id ) )->weight ) * alice_comment_vote_rewards ) / alice_comment.total_vote_weight ), BMT_SYMBOL );
      reward_bmt += alice_comment_vote_rewards - ( alice_vote_alice_reward + bob_vote_alice_reward ).amount.value;

      auto bob_comment_reward = ( ( reward_bmt * bob_comment.net_rshares.value * bob_comment.net_rshares.value ) / total_rshares2 ).to_uint64();
      total_rshares2 -= uint128_t( bob_comment.net_rshares.value ) * bob_comment.net_rshares.value;
      reward_bmt -= bob_comment_reward;
      auto bob_comment_vote_rewards = bob_comment_reward / 2;
      bob_comment_reward -= bob_comment_vote_rewards;

      auto alice_vote_bob_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id, db.get_account( "alice" ) ).id ) )->weight ) * bob_comment_vote_rewards ) / bob_comment.total_vote_weight ), BMT_SYMBOL );
      auto bob_vote_bob_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id, db.get_account( "bob" ) ).id ) )->weight ) * bob_comment_vote_rewards ) / bob_comment.total_vote_weight ), BMT_SYMBOL );
      auto sam_vote_bob_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "bob", string( "test" ).id, db.get_account( "sam" ) ).id ) )->weight ) * bob_comment_vote_rewards ) / bob_comment.total_vote_weight ), BMT_SYMBOL );
      reward_bmt += bob_comment_vote_rewards - ( alice_vote_bob_reward + bob_vote_bob_reward + sam_vote_bob_reward ).amount.value;

      auto dave_comment_reward = ( ( reward_bmt * dave_comment.net_rshares.value * dave_comment.net_rshares.value ) / total_rshares2 ).to_uint64();
      total_rshares2 -= uint128_t( dave_comment.net_rshares.value ) * dave_comment.net_rshares.value;
      reward_bmt -= dave_comment_reward;
      auto dave_comment_vote_rewards = dave_comment_reward / 2;
      dave_comment_reward -= dave_comment_vote_rewards;

      auto bob_vote_dave_reward = asset( static_cast< uint64_t >( ( u256( vote_idx.find( std::make_tuple( db.get_comment( "dave", string( "test" ).id, db.get_account( "bob" ) ).id ) )->weight ) * dave_comment_vote_rewards ) / dave_comment.total_vote_weight ), BMT_SYMBOL );
      reward_bmt += dave_comment_vote_rewards - bob_vote_dave_reward.amount.value;

      // Calculate rewards paid to parent posts
      auto alice_pays_alice_vest = alice_comment_reward;
      auto bob_pays_bob_vest = bob_comment_reward;
      auto dave_pays_dave_vest = dave_comment_reward;

      auto bob_pays_alice_vest = bob_pays_bob_vest / 2;
      bob_pays_bob_vest -= bob_pays_alice_vest;

      auto dave_pays_sam_vest = dave_pays_dave_vest / 2;
      dave_pays_dave_vest -= dave_pays_sam_vest;
      auto dave_pays_bob_vest = dave_pays_sam_vest / 2;
      dave_pays_sam_vest -= dave_pays_bob_vest;
      auto dave_pays_alice_vest = dave_pays_bob_vest / 2;
      dave_pays_bob_vest -= dave_pays_alice_vest;

      // Calculate total comment payouts
      auto alice_comment_total_payout = asset( alice_pays_alice_vest, BMT_SYMBOL );
      alice_comment_total_payout += asset( bob_pays_alice_vest, BMT_SYMBOL );
      alice_comment_total_payout += asset( dave_pays_alice_vest, BMT_SYMBOL );
      auto bob_comment_total_payout = asset( bob_pays_bob_vest, BMT_SYMBOL );
      bob_comment_total_payout += asset( dave_pays_bob_vest, BMT_SYMBOL );
      auto sam_comment_total_payout = asset( dave_pays_sam_vest, BMT_SYMBOL );
      auto dave_comment_total_payout = asset( dave_pays_dave_vest, BMT_SYMBOL );

      auto alice_starting_rep = db.get_account( "alice" ).rep_shares;
      auto bob_starting_rep = db.get_account( "bob" ).rep_shares;
      auto sam_starting_rep = db.get_account( "sam" ).rep_shares;
      auto dave_starting_rep = db.get_account( "dave" ).rep_shares;

      generate_block();

      gpo = db.get_dynamic_global_properties();

      // Calculate vesting share rewards from voting.
      auto alice_vote_alice_rep = alice_vote_alice_reward * gpo.get_rep_share_price();
      auto bob_vote_alice_rep = bob_vote_alice_reward * gpo.get_rep_share_price();
      auto alice_vote_bob_rep = alice_vote_bob_reward * gpo.get_rep_share_price();
      auto bob_vote_bob_rep = bob_vote_bob_reward * gpo.get_rep_share_price();
      auto sam_vote_bob_rep = sam_vote_bob_reward * gpo.get_rep_share_price();
      auto bob_vote_dave_rep = bob_vote_dave_reward * gpo.get_rep_share_price();

      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).total_payout_value.amount.value == alice_comment_total_payout.amount.value );
      BOOST_REQUIRE( db.get_comment( "bob", string( "test" ) ).total_payout_value.amount.value == bob_comment_total_payout.amount.value );
      BOOST_REQUIRE( db.get_comment( "sam", string( "test" ) ).total_payout_value.amount.value == sam_comment_total_payout.amount.value );
      BOOST_REQUIRE( db.get_comment( "dave", string( "test" ) ).total_payout_value.amount.value == dave_comment_total_payout.amount.value );

      // ops 0-3, 5-6, and 10 are comment reward ops
      auto ops = get_last_operations( 13 );

      BOOST_TEST_MESSAGE( "Checking Virtual Operation Correctness" );

      curate_reward_operation cur_vop;
      comment_reward_operation com_vop = ops[0].get< comment_reward_operation >();

      BOOST_REQUIRE( com_vop.author == "alice" );
      BOOST_REQUIRE( com_vop.permlink == "test" );
      BOOST_REQUIRE( com_vop.originating_author == "dave" );
      BOOST_REQUIRE( com_vop.originating_permlink == "test" );
      BOOST_REQUIRE( ( com_vop.rep_payout * gpo.get_rep_share_price() ).amount.value == dave_pays_alice_vest );

      com_vop = ops[1].get< comment_reward_operation >();
      BOOST_REQUIRE( com_vop.author == "bob" );
      BOOST_REQUIRE( com_vop.permlink == "test" );
      BOOST_REQUIRE( com_vop.originating_author == "dave" );
      BOOST_REQUIRE( com_vop.originating_permlink == "test" );
      BOOST_REQUIRE( ( com_vop.rep_payout * gpo.get_rep_share_price() ).amount.value == dave_pays_bob_vest );

      com_vop = ops[2].get< comment_reward_operation >();
      BOOST_REQUIRE( com_vop.author == "sam" );
      BOOST_REQUIRE( com_vop.permlink == "test" );
      BOOST_REQUIRE( com_vop.originating_author == "dave" );
      BOOST_REQUIRE( com_vop.originating_permlink == "test" );
      BOOST_REQUIRE( ( com_vop.rep_payout * gpo.get_rep_share_price() ).amount.value == dave_pays_sam_vest );

      com_vop = ops[3].get< comment_reward_operation >();
      BOOST_REQUIRE( com_vop.author == "dave" );
      BOOST_REQUIRE( com_vop.permlink == "test" );
      BOOST_REQUIRE( com_vop.originating_author == "dave" );
      BOOST_REQUIRE( com_vop.originating_permlink == "test" );
      BOOST_REQUIRE( ( com_vop.rep_payout * gpo.get_rep_share_price() ).amount.value == dave_pays_dave_vest );

      cur_vop = ops[4].get< curate_reward_operation >();
      BOOST_REQUIRE( cur_vop.curator == "bob" );
      BOOST_REQUIRE( cur_vop.reward.amount.value == bob_vote_dave_rep.amount.value );
      BOOST_REQUIRE( cur_vop.comment_author == "dave" );
      BOOST_REQUIRE( cur_vop.comment_permlink == "test" );

      com_vop = ops[5].get< comment_reward_operation >();
      BOOST_REQUIRE( com_vop.author == "alice" );
      BOOST_REQUIRE( com_vop.permlink == "test" );
      BOOST_REQUIRE( com_vop.originating_author == "bob" );
      BOOST_REQUIRE( com_vop.originating_permlink == "test" );
      BOOST_REQUIRE( ( com_vop.rep_payout * gpo.get_rep_share_price() ).amount.value == bob_pays_alice_vest );

      com_vop = ops[6].get< comment_reward_operation >();
      BOOST_REQUIRE( com_vop.author == "bob" );
      BOOST_REQUIRE( com_vop.permlink == "test" );
      BOOST_REQUIRE( com_vop.originating_author == "bob" );
      BOOST_REQUIRE( com_vop.originating_permlink == "test" );
      BOOST_REQUIRE( ( com_vop.rep_payout * gpo.get_rep_share_price() ).amount.value == bob_pays_bob_vest );

      cur_vop = ops[7].get< curate_reward_operation >();
      BOOST_REQUIRE( cur_vop.curator == "sam" );
      BOOST_REQUIRE( cur_vop.reward.amount.value == sam_vote_bob_rep.amount.value );
      BOOST_REQUIRE( cur_vop.comment_author == "bob" );
      BOOST_REQUIRE( cur_vop.comment_permlink == "test" );

      cur_vop = ops[8].get< curate_reward_operation >();
      BOOST_REQUIRE( cur_vop.curator == "bob" );
      BOOST_REQUIRE( cur_vop.reward.amount.value == bob_vote_bob_rep.amount.value );
      BOOST_REQUIRE( cur_vop.comment_author == "bob" );
      BOOST_REQUIRE( cur_vop.comment_permlink == "test" );

      cur_vop = ops[9].get< curate_reward_operation >();
      BOOST_REQUIRE( cur_vop.curator == "alice" );
      BOOST_REQUIRE( cur_vop.reward.amount.value == alice_vote_bob_rep.amount.value );
      BOOST_REQUIRE( cur_vop.comment_author == "bob" );
      BOOST_REQUIRE( cur_vop.comment_permlink == "test" );

      com_vop = ops[10].get< comment_reward_operation >();
      BOOST_REQUIRE( com_vop.author == "alice" );
      BOOST_REQUIRE( com_vop.permlink == "test" );
      BOOST_REQUIRE( com_vop.originating_author == "alice" );
      BOOST_REQUIRE( com_vop.originating_permlink == "test" );
      BOOST_REQUIRE( ( com_vop.rep_payout * gpo.get_rep_share_price() ).amount.value == alice_pays_alice_vest );

      cur_vop = ops[11].get< curate_reward_operation >();
      BOOST_REQUIRE( cur_vop.curator == "bob" );
      BOOST_REQUIRE( cur_vop.reward.amount.value == bob_vote_alice_rep.amount.value );
      BOOST_REQUIRE( cur_vop.comment_author == "alice" );
      BOOST_REQUIRE( cur_vop.comment_permlink == "test" );

      cur_vop = ops[12].get< curate_reward_operation >();
      BOOST_REQUIRE( cur_vop.curator == "alice" );
      BOOST_REQUIRE( cur_vop.reward.amount.value == alice_vote_alice_rep.amount.value );
      BOOST_REQUIRE( cur_vop.comment_author == "alice" );
      BOOST_REQUIRE( cur_vop.comment_permlink == "test" );

      BOOST_TEST_MESSAGE( "Checking account balances" );

      auto alice_total_rep = alice_starting_rep + asset( alice_pays_alice_vest + bob_pays_alice_vest + dave_pays_alice_vest + alice_vote_alice_reward.amount + alice_vote_bob_reward.amount, BMT_SYMBOL ) * gpo.get_rep_share_price();
      BOOST_REQUIRE( db.get_account( "alice" ).rep_shares.amount.value == alice_total_rep.amount.value );

      auto bob_total_rep = bob_starting_rep + asset( bob_pays_bob_vest + dave_pays_bob_vest + bob_vote_alice_reward.amount + bob_vote_bob_reward.amount + bob_vote_dave_reward.amount, BMT_SYMBOL ) * gpo.get_rep_share_price();
      BOOST_REQUIRE( db.get_account( "bob" ).rep_shares.amount.value == bob_total_rep.amount.value );

      auto sam_total_rep = bob_starting_rep + asset( dave_pays_sam_vest + sam_vote_bob_reward.amount, BMT_SYMBOL ) * gpo.get_rep_share_price();
      BOOST_REQUIRE( db.get_account( "sam" ).rep_shares.amount.value == sam_total_rep.amount.value );
      auto dave_total_rep = dave_starting_rep + asset( dave_pays_dave_vest, BMT_SYMBOL ) * gpo.get_rep_share_price();
      BOOST_REQUIRE( db.get_account( "dave" ).rep_shares.amount.value == dave_total_rep.amount.value );
   }
   FC_LOG_AND_RETHROW()
}
*/

BOOST_AUTO_TEST_CASE( rep_withdraw_route )
{
   try
   {
      ACTORS( (alice)(bob)(sam) )

      auto original_rep = alice.rep_shares;

      fund( "alice", 1040000 );
      vest( "alice", 1040000 );

      auto withdraw_amount = alice.rep_shares - original_rep;

      BOOST_TEST_MESSAGE( "Setup vesting withdraw" );
      withdraw_rep_operation wv;
      wv.account = "alice";
      wv.rep_shares = withdraw_amount;

      signed_transaction tx;
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.operations.push_back( wv );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();

      BOOST_TEST_MESSAGE( "Setting up bob destination" );
      set_withdraw_rep_route_operation op;
      op.from_account = "alice";
      op.to_account = "bob";
      op.percent = BMCHAIN_1_PERCENT * 50;
      op.auto_vest = true;
      tx.operations.push_back( op );

      BOOST_TEST_MESSAGE( "Setting up sam destination" );
      op.to_account = "sam";
      op.percent = BMCHAIN_1_PERCENT * 30;
      op.auto_vest = false;
      tx.operations.push_back( op );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_TEST_MESSAGE( "Setting up first withdraw" );

      auto rep_withdraw_rate = alice.rep_withdraw_rate;
      auto old_alice_balance = alice.balance;
      auto old_alice_rep = alice.rep_shares;
      auto old_bob_balance = bob.balance;
      auto old_bob_rep = bob.rep_shares;
      auto old_sam_balance = sam.balance;
      auto old_sam_rep = sam.rep_shares;
      generate_blocks( alice.next_rep_withdrawal, true );

      {
         const auto& alice = db.get_account( "alice" );
         const auto& bob = db.get_account( "bob" );
         const auto& sam = db.get_account( "sam" );

         BOOST_REQUIRE( alice.rep_shares == old_alice_rep - rep_withdraw_rate );
         BOOST_REQUIRE( alice.balance == old_alice_balance + asset( ( rep_withdraw_rate.amount * BMCHAIN_1_PERCENT * 20 ) / BMCHAIN_100_PERCENT, REP_SYMBOL ) * db.get_dynamic_global_properties().get_rep_share_price() );
         BOOST_REQUIRE( bob.rep_shares == old_bob_rep + asset( ( rep_withdraw_rate.amount * BMCHAIN_1_PERCENT * 50 ) / BMCHAIN_100_PERCENT, REP_SYMBOL ) );
         BOOST_REQUIRE( bob.balance == old_bob_balance );
         BOOST_REQUIRE( sam.rep_shares == old_sam_rep );
         BOOST_REQUIRE( sam.balance ==  old_sam_balance + asset( ( rep_withdraw_rate.amount * BMCHAIN_1_PERCENT * 30 ) / BMCHAIN_100_PERCENT, REP_SYMBOL ) * db.get_dynamic_global_properties().get_rep_share_price() );

         old_alice_balance = alice.balance;
         old_alice_rep = alice.rep_shares;
         old_bob_balance = bob.balance;
         old_bob_rep = bob.rep_shares;
         old_sam_balance = sam.balance;
         old_sam_rep = sam.rep_shares;
      }

      BOOST_TEST_MESSAGE( "Test failure with greater than 100% destination assignment" );

      tx.operations.clear();
      tx.signatures.clear();

      op.to_account = "sam";
      op.percent = BMCHAIN_1_PERCENT * 50 + 1;
      tx.operations.push_back( op );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), fc::exception );

      BOOST_TEST_MESSAGE( "Test from_account receiving no withdraw" );

      tx.operations.clear();
      tx.signatures.clear();

      op.to_account = "sam";
      op.percent = BMCHAIN_1_PERCENT * 50;
      tx.operations.push_back( op );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      generate_blocks( db.get_account( "alice" ).next_rep_withdrawal, true );
      {
         const auto& alice = db.get_account( "alice" );
         const auto& bob = db.get_account( "bob" );
         const auto& sam = db.get_account( "sam" );

         BOOST_REQUIRE( alice.rep_shares == old_alice_rep - rep_withdraw_rate );
         BOOST_REQUIRE( alice.balance == old_alice_balance );
         BOOST_REQUIRE( bob.rep_shares == old_bob_rep + asset( ( rep_withdraw_rate.amount * BMCHAIN_1_PERCENT * 50 ) / BMCHAIN_100_PERCENT, REP_SYMBOL ) );
         BOOST_REQUIRE( bob.balance == old_bob_balance );
         BOOST_REQUIRE( sam.rep_shares == old_sam_rep );
         BOOST_REQUIRE( sam.balance ==  old_sam_balance + asset( ( rep_withdraw_rate.amount * BMCHAIN_1_PERCENT * 50 ) / BMCHAIN_100_PERCENT, REP_SYMBOL ) * db.get_dynamic_global_properties().get_rep_share_price() );
      }
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( bmt_inflation )
{
   try
   {
   /*
      BOOST_TEST_MESSAGE( "Testing BMT Inflation until the vesting start block" );

      auto gpo = db.get_dynamic_global_properties();
      auto virtual_supply = gpo.virtual_supply;
      auto witness_name = db.get_scheduled_witness(1);
      auto old_witness_balance = db.get_account( witness_name ).balance;
      auto old_witness_shares = db.get_account( witness_name ).rep_shares;

      auto new_rewards = std::max( BMCHAIN_MIN_CONTENT_REWARD, asset( ( BMCHAIN_CONTENT_APR * gpo.virtual_supply.amount ) / ( BMCHAIN_BLOCKS_PER_YEAR * 100 ), BMT_SYMBOL ) )
         + std::max( BMCHAIN_MIN_CURATE_REWARD, asset( ( BMCHAIN_CURATE_APR * gpo.virtual_supply.amount ) / ( BMCHAIN_BLOCKS_PER_YEAR * 100 ), BMT_SYMBOL ) );
      auto witness_pay = std::max( BMCHAIN_MIN_PRODUCER_REWARD, asset( ( BMCHAIN_PRODUCER_APR * gpo.virtual_supply.amount ) / ( BMCHAIN_BLOCKS_PER_YEAR * 100 ), BMT_SYMBOL ) );
      auto witness_pay_shares = asset( 0, REP_SYMBOL );
      auto new_rep_bmt = asset( 0, BMT_SYMBOL );
      auto new_rep_shares = gpo.total_rep_shares;

      if ( db.get_account( witness_name ).rep_shares.amount.value == 0 )
      {
         new_rep_bmt += witness_pay;
         new_rep_shares += witness_pay * ( gpo.total_rep_shares / gpo.total_rep_fund_bmt );
      }

      auto new_supply = gpo.current_supply + new_rewards + witness_pay + new_rep_bmt;
      new_rewards += gpo.total_reward_fund_bmt;
      new_rep_bmt += gpo.total_rep_fund_bmt;

      generate_block();

      gpo = db.get_dynamic_global_properties();

      BOOST_REQUIRE( gpo.current_supply.amount.value == new_supply.amount.value );
      BOOST_REQUIRE( gpo.virtual_supply.amount.value == new_supply.amount.value );
      BOOST_REQUIRE( gpo.total_reward_fund_bmt.amount.value == new_rewards.amount.value );
      BOOST_REQUIRE( gpo.total_rep_fund_bmt.amount.value == new_rep_bmt.amount.value );
      BOOST_REQUIRE( gpo.total_rep_shares.amount.value == new_rep_shares.amount.value );
      BOOST_REQUIRE( db.get_account( witness_name ).balance.amount.value == ( old_witness_balance + witness_pay ).amount.value );

      validate_database();

      BOOST_TEST_MESSAGE( "Testing up to the start block for miner voting" );

      while( db.head_block_num() < BMCHAIN_START_MINER_VOTING_BLOCK - 1 )
      {
         virtual_supply = gpo.virtual_supply;
         witness_name = db.get_scheduled_witness(1);
         old_witness_balance = db.get_account( witness_name ).balance;

         new_rewards = std::max( BMCHAIN_MIN_CONTENT_REWARD, asset( ( BMCHAIN_CONTENT_APR * gpo.virtual_supply.amount ) / ( BMCHAIN_BLOCKS_PER_YEAR * 100 ), BMT_SYMBOL ) )
            + std::max( BMCHAIN_MIN_CURATE_REWARD, asset( ( BMCHAIN_CURATE_APR * gpo.virtual_supply.amount ) / ( BMCHAIN_BLOCKS_PER_YEAR * 100 ), BMT_SYMBOL ) );
         witness_pay = std::max( BMCHAIN_MIN_PRODUCER_REWARD, asset( ( BMCHAIN_PRODUCER_APR * gpo.virtual_supply.amount ) / ( BMCHAIN_BLOCKS_PER_YEAR * 100 ), BMT_SYMBOL ) );
         auto witness_pay_shares = asset( 0, REP_SYMBOL );
         new_rep_bmt = asset( ( witness_pay + new_rewards ).amount * 9, BMT_SYMBOL );
         new_rep_shares = gpo.total_rep_shares;

         if ( db.get_account( witness_name ).rep_shares.amount.value == 0 )
         {
            new_rep_bmt += witness_pay;
            witness_pay_shares = witness_pay * gpo.get_rep_share_price();
            new_rep_shares += witness_pay_shares;
            new_supply += witness_pay;
            witness_pay = asset( 0, BMT_SYMBOL );
         }

         new_supply = gpo.current_supply + new_rewards + witness_pay + new_rep_bmt;
         new_rewards += gpo.total_reward_fund_bmt;
         new_rep_bmt += gpo.total_rep_fund_bmt;

         generate_block();

         gpo = db.get_dynamic_global_properties();

         BOOST_REQUIRE( gpo.current_supply.amount.value == new_supply.amount.value );
         BOOST_REQUIRE( gpo.virtual_supply.amount.value == new_supply.amount.value );
         BOOST_REQUIRE( gpo.total_reward_fund_bmt.amount.value == new_rewards.amount.value );
         BOOST_REQUIRE( gpo.total_rep_fund_bmt.amount.value == new_rep_bmt.amount.value );
         BOOST_REQUIRE( gpo.total_rep_shares.amount.value == new_rep_shares.amount.value );
         BOOST_REQUIRE( db.get_account( witness_name ).balance.amount.value == ( old_witness_balance + witness_pay ).amount.value );
         BOOST_REQUIRE( db.get_account( witness_name ).rep_shares.amount.value == ( old_witness_shares + witness_pay_shares ).amount.value );

         validate_database();
      }

      for( int i = 0; i < BMCHAIN_BLOCKS_PER_DAY; i++ )
      {
         virtual_supply = gpo.virtual_supply;
         witness_name = db.get_scheduled_witness(1);
         old_witness_balance = db.get_account( witness_name ).balance;

         new_rewards = std::max( BMCHAIN_MIN_CONTENT_REWARD, asset( ( BMCHAIN_CONTENT_APR * gpo.virtual_supply.amount ) / ( BMCHAIN_BLOCKS_PER_YEAR * 100 ), BMT_SYMBOL ) )
            + std::max( BMCHAIN_MIN_CURATE_REWARD, asset( ( BMCHAIN_CURATE_APR * gpo.virtual_supply.amount ) / ( BMCHAIN_BLOCKS_PER_YEAR * 100 ), BMT_SYMBOL ) );
         witness_pay = std::max( BMCHAIN_MIN_PRODUCER_REWARD, asset( ( BMCHAIN_PRODUCER_APR * gpo.virtual_supply.amount ) / ( BMCHAIN_BLOCKS_PER_YEAR * 100 ), BMT_SYMBOL ) );
         witness_pay_shares = witness_pay * gpo.get_rep_share_price();
         new_rep_bmt = asset( ( witness_pay + new_rewards ).amount * 9, BMT_SYMBOL ) + witness_pay;
         new_rep_shares = gpo.total_rep_shares + witness_pay_shares;
         new_supply = gpo.current_supply + new_rewards + new_rep_bmt;
         new_rewards += gpo.total_reward_fund_bmt;
         new_rep_bmt += gpo.total_rep_fund_bmt;

         generate_block();

         gpo = db.get_dynamic_global_properties();

         BOOST_REQUIRE( gpo.current_supply.amount.value == new_supply.amount.value );
         BOOST_REQUIRE( gpo.virtual_supply.amount.value == new_supply.amount.value );
         BOOST_REQUIRE( gpo.total_reward_fund_bmt.amount.value == new_rewards.amount.value );
         BOOST_REQUIRE( gpo.total_rep_fund_bmt.amount.value == new_rep_bmt.amount.value );
         BOOST_REQUIRE( gpo.total_rep_shares.amount.value == new_rep_shares.amount.value );
         BOOST_REQUIRE( db.get_account( witness_name ).rep_shares.amount.value == ( old_witness_shares + witness_pay_shares ).amount.value );

         validate_database();
      }

      virtual_supply = gpo.virtual_supply;
      rep_shares = gpo.total_rep_shares;
      rep_bmt = gpo.total_rep_fund_bmt;
      reward_bmt = gpo.total_reward_fund_bmt;

      witness_name = db.get_scheduled_witness(1);
      old_witness_shares = db.get_account( witness_name ).rep_shares;

      generate_block();

      gpo = db.get_dynamic_global_properties();

      BOOST_REQUIRE_EQUAL( gpo.total_rep_fund_bmt.amount.value,
         ( rep_bmt.amount.value
            + ( ( ( uint128_t( virtual_supply.amount.value ) / 10 ) / BMCHAIN_BLOCKS_PER_YEAR ) * 9 )
            + ( uint128_t( virtual_supply.amount.value ) / 100 / BMCHAIN_BLOCKS_PER_YEAR ) ).to_uint64() );
      BOOST_REQUIRE_EQUAL( gpo.total_reward_fund_bmt.amount.value,
         reward_bmt.amount.value + virtual_supply.amount.value / 10 / BMCHAIN_BLOCKS_PER_YEAR + virtual_supply.amount.value / 10 / BMCHAIN_BLOCKS_PER_DAY );
      BOOST_REQUIRE_EQUAL( db.get_account( witness_name ).rep_shares.amount.value,
         old_witness_shares.amount.value + ( asset( ( ( virtual_supply.amount.value / BMCHAIN_BLOCKS_PER_YEAR ) * BMCHAIN_1_PERCENT ) / BMCHAIN_100_PERCENT, BMT_SYMBOL ) * ( rep_shares / rep_bmt ) ).amount.value );
      validate_database();
      */
   }
   FC_LOG_AND_RETHROW();
}

BOOST_AUTO_TEST_CASE( post_rate_limit )
{
   try
   {
      ACTORS( (alice) )

      fund( "alice", 10000 );
      vest( "alice", 10000 );

      comment_operation op;
      op.author = "alice";
      op.permlink = "test1";
      op.parent_author = "";
      op.parent_permlink = "test";
      op.body = "test";

      signed_transaction tx;

      tx.operations.push_back( op );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_REQUIRE( db.get_comment( "alice", string( "test1" ) ).reward_weight == BMCHAIN_100_PERCENT );

      tx.operations.clear();
      tx.signatures.clear();

      generate_blocks( db.head_block_time() + BMCHAIN_MIN_ROOT_COMMENT_INTERVAL + fc::seconds( BMCHAIN_BLOCK_INTERVAL ), true );

      op.permlink = "test2";

      tx.operations.push_back( op );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_REQUIRE( db.get_comment( "alice", string( "test2" ) ).reward_weight == BMCHAIN_100_PERCENT );

      generate_blocks( db.head_block_time() + BMCHAIN_MIN_ROOT_COMMENT_INTERVAL + fc::seconds( BMCHAIN_BLOCK_INTERVAL ), true );

      tx.operations.clear();
      tx.signatures.clear();

      op.permlink = "test3";

      tx.operations.push_back( op );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_REQUIRE( db.get_comment( "alice", string( "test3" ) ).reward_weight == BMCHAIN_100_PERCENT );

      generate_blocks( db.head_block_time() + BMCHAIN_MIN_ROOT_COMMENT_INTERVAL + fc::seconds( BMCHAIN_BLOCK_INTERVAL ), true );

      tx.operations.clear();
      tx.signatures.clear();

      op.permlink = "test4";

      tx.operations.push_back( op );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_REQUIRE( db.get_comment( "alice", string( "test4" ) ).reward_weight == BMCHAIN_100_PERCENT );

      generate_blocks( db.head_block_time() + BMCHAIN_MIN_ROOT_COMMENT_INTERVAL + fc::seconds( BMCHAIN_BLOCK_INTERVAL ), true );

      tx.operations.clear();
      tx.signatures.clear();

      op.permlink = "test5";

      tx.operations.push_back( op );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_REQUIRE( db.get_comment( "alice", string( "test5" ) ).reward_weight == BMCHAIN_100_PERCENT );
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( comment_freeze )
{
   try
   {
      ACTORS( (alice)(bob)(sam)(dave) )
      fund( "alice", 10000 );
      fund( "bob", 10000 );
      fund( "sam", 10000 );
      fund( "dave", 10000 );

      vest( "alice", 10000 );
      vest( "bob", 10000 );
      vest( "sam", 10000 );
      vest( "dave", 10000 );

      //auto exchange_rate = price( ASSET( "1.250 TESTS" ), ASSET( "1.000 TBD" ) );
      //set_price_feed( exchange_rate );

      signed_transaction tx;

      comment_operation comment;
      comment.author = "alice";
      comment.parent_author = "";
      comment.permlink = "test";
      comment.parent_permlink = "test";
      comment.body = "test";

      tx.operations.push_back( comment );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      comment.body = "test2";

      tx.operations.clear();
      tx.signatures.clear();

      tx.operations.push_back( comment );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      vote_operation vote;
      vote.weight = BMCHAIN_100_PERCENT;
      vote.voter = "bob";
      vote.author = "alice";
      vote.permlink = "test";

      tx.operations.clear();
      tx.signatures.clear();

      tx.operations.push_back( vote );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).last_payout == fc::time_point_sec::min() );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).cashout_time != fc::time_point_sec::min() );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).cashout_time != fc::time_point_sec::maximum() );

      generate_blocks( db.get_comment( "alice", string( "test" ) ).cashout_time, true );

      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).last_payout == db.head_block_time() );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).cashout_time == fc::time_point_sec::maximum() );

      vote.voter = "sam";

      tx.operations.clear();
      tx.signatures.clear();

      tx.operations.push_back( vote );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( sam_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).cashout_time == fc::time_point_sec::maximum() );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).net_rshares.value == 0 );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).abs_rshares.value == 0 );

      vote.voter = "bob";
      vote.weight = BMCHAIN_100_PERCENT * -1;

      tx.operations.clear();
      tx.signatures.clear();

      tx.operations.push_back( vote );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( bob_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );

      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).cashout_time == fc::time_point_sec::maximum() );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).net_rshares.value == 0 );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).abs_rshares.value == 0 );

      vote.voter = "dave";
      vote.weight = 0;

      tx.operations.clear();
      tx.signatures.clear();

      tx.operations.push_back( vote );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( dave_private_key, db.get_chain_id() );

      db.push_transaction( tx, 0 );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).cashout_time == fc::time_point_sec::maximum() );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).net_rshares.value == 0 );
      BOOST_REQUIRE( db.get_comment( "alice", string( "test" ) ).abs_rshares.value == 0 );

      comment.body = "test4";

      tx.operations.clear();
      tx.signatures.clear();

      tx.operations.push_back( comment );
      tx.sign( alice_private_key, db.get_chain_id() );
      BMCHAIN_REQUIRE_THROW( db.push_transaction( tx, 0 ), fc::exception );
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( clear_null_account )
{
   try
   {
      BOOST_TEST_MESSAGE( "Testing clearing the null account's balances on block" );

      ACTORS( (alice) );
      generate_block();

      //set_price_feed( price( ASSET( "1.000 TESTS" ), ASSET( "1.000 TBD" ) ) );

      fund( "alice", ASSET( "10.000 TESTS" ) );
      fund( "alice", ASSET( "10.000 TBD" ) );

      transfer_operation transfer1;
      transfer1.from = "alice";
      transfer1.to = BMCHAIN_NULL_ACCOUNT;
      transfer1.amount = ASSET( "1.000 TESTS" );

      transfer_operation transfer2;
      transfer2.from = "alice";
      transfer2.to = BMCHAIN_NULL_ACCOUNT;
      transfer2.amount = ASSET( "2.000 TBD" );

      transfer_to_rep_operation vest;
      vest.from = "alice";
      vest.to = BMCHAIN_NULL_ACCOUNT;
      vest.amount = ASSET( "3.000 TESTS" );

      transfer_to_savings_operation save1;
      save1.from = "alice";
      save1.to = BMCHAIN_NULL_ACCOUNT;
      save1.amount = ASSET( "4.000 TESTS" );

      transfer_to_savings_operation save2;
      save2.from = "alice";
      save2.to = BMCHAIN_NULL_ACCOUNT;
      save2.amount = ASSET( "5.000 TBD" );

      BOOST_TEST_MESSAGE( "--- Transferring to NULL Account" );

      signed_transaction tx;
      tx.operations.push_back( transfer1 );
      tx.operations.push_back( transfer2 );
      tx.operations.push_back( vest );
      tx.operations.push_back( save1);
      tx.operations.push_back( save2 );
      tx.set_expiration( db.head_block_time() + BMCHAIN_MAX_TIME_UNTIL_EXPIRATION );
      tx.sign( alice_private_key, db.get_chain_id() );
      db.push_transaction( tx, 0 );
      validate_database();

      db_plugin->debug_update( [=]( database& db )
      {
         db.modify( db.get_account( BMCHAIN_NULL_ACCOUNT ), [&]( account_object& a )
         {
            a.reward_bmt_balance = ASSET( "1.000 TESTS" );
            a.reward_rep_balance = ASSET( "1.000000 VESTS" );
            a.reward_rep_bmt = ASSET( "1.000 TESTS" );
         });

         db.modify( db.get_dynamic_global_properties(), [&]( dynamic_global_property_object& gpo )
         {
            gpo.current_supply += ASSET( "2.000 TESTS" );
            gpo.virtual_supply += ASSET( "3.000 TESTS" );
            gpo.pending_rewarded_rep_shares += ASSET( "1.000000 VESTS" );
            gpo.pending_rewarded_rep_bmt += ASSET( "1.000 TESTS" );
         });
      });

      validate_database();

      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).balance == ASSET( "1.000 TESTS" ) );
      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).rep_shares > ASSET( "0.000000 VESTS" ) );
      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).savings_balance == ASSET( "4.000 TESTS" ) );
      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).reward_bmt_balance == ASSET( "1.000 TESTS" ) );
      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).reward_rep_balance == ASSET( "1.000000 VESTS" ) );
      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).reward_rep_bmt == ASSET( "1.000 TESTS" ) );
      BOOST_REQUIRE( db.get_account( "alice" ).balance == ASSET( "2.000 TESTS" ) );

      BOOST_TEST_MESSAGE( "--- Generating block to clear balances" );
      generate_block();
      validate_database();

      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).balance == ASSET( "0.000 TESTS" ) );
      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).rep_shares == ASSET( "0.000000 VESTS" ) );
      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).savings_balance == ASSET( "0.000 TESTS" ) );
      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).reward_bmt_balance == ASSET( "0.000 TESTS" ) );
      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).reward_rep_balance == ASSET( "0.000000 VESTS" ) );
      BOOST_REQUIRE( db.get_account( BMCHAIN_NULL_ACCOUNT ).reward_rep_bmt == ASSET( "0.000 TESTS" ) );
      BOOST_REQUIRE( db.get_account( "alice" ).balance == ASSET( "2.000 TESTS" ) );
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_SUITE_END()
#endif
