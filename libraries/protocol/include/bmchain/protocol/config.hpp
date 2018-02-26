/*
 * Copyright (c) 2017 BMChain, Inc., and contributors.
 */
#pragma once

#define BMCHAIN_BLOCKCHAIN_VERSION              ( version(0, 0, 0) )
#define BMCHAIN_BLOCKCHAIN_HARDFORK_VERSION     ( hardfork_version( BMCHAIN_BLOCKCHAIN_VERSION ) )

#ifdef IS_TEST_NET
#define BMCHAIN_INIT_PRIVATE_KEY                (fc::ecc::private_key::regenerate(fc::sha256::hash(std::string("init_key"))))
#define BMCHAIN_INIT_PUBLIC_KEY_STR             (std::string( bmchain::protocol::public_key_type(BMCHAIN_INIT_PRIVATE_KEY.get_public_key()) ))
#define BMCHAIN_CHAIN_ID                        (fc::sha256::hash("testnet"))

#define REP_SYMBOL  (uint64_t(6) | (uint64_t('V') << 8) | (uint64_t('E') << 16) | (uint64_t('S') << 24) | (uint64_t('T') << 32) | (uint64_t('S') << 40)) ///< VESTS with 6 digits of precision
#define BMT_SYMBOL  (uint64_t(3) | (uint64_t('T') << 8) | (uint64_t('E') << 16) | (uint64_t('S') << 24) | (uint64_t('T') << 32) | (uint64_t('S') << 40)) ///< BMT with 3 digits of precision

#define BMCHAIN_SYMBOL                          "TEST"
#define BMCHAIN_ADDRESS_PREFIX                  "TST"

#define BMCHAIN_GENESIS_TIME                    (fc::time_point_sec(1451606400))
#define BMCHAIN_CASHOUT_WINDOW_SECONDS          (60*10) /// 1 hr /// bmchain
#define BMCHAIN_SECOND_CASHOUT_WINDOW           (60*60*24*3) /// 3 days
#define BMCHAIN_UPVOTE_LOCKOUT             (fc::minutes(5))

#define BMCHAIN_MIN_ACCOUNT_CREATION_FEE          0

#define BMCHAIN_OWNER_AUTH_RECOVERY_PERIOD                  fc::seconds(60)
#define BMCHAIN_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD  fc::seconds(12)
#define BMCHAIN_OWNER_UPDATE_LIMIT                          fc::seconds(0)
#define BMCHAIN_OWNER_AUTH_HISTORY_TRACKING_START_BLOCK_NUM 1
#else // IS LIVE BMT NETWORK

#define BMCHAIN_INIT_PUBLIC_KEY_STR             "BMT76G5486bhLKXkXgN1nSkeDbpWEaiavF7i6tDULdpvyr69CLbHC"
#define BMCHAIN_CHAIN_ID                        (fc::sha256::hash("bmchain"))
#define REP_SYMBOL  (uint64_t(6) | (uint64_t('R') << 8) | (uint64_t('E') << 16) | (uint64_t('P') << 24) ) ///< RPT with 6 digits of precision
#define BMT_SYMBOL  (uint64_t(3) | (uint64_t('B') << 8) | (uint64_t('M') << 16) | (uint64_t('T') << 24))  ///< BMT with 3 digits of precisionDollars with 3 digits of precision
#define BMCHAIN_SYMBOL                          "BMT"
#define BMCHAIN_ADDRESS_PREFIX                  "BMT"

#define BMCHAIN_GENESIS_TIME                    (fc::time_point_sec(1510655500)) /// 11/14/2017
#define BMCHAIN_CASHOUT_WINDOW_SECONDS          (60*60*24*7)  /// 7 days
#define BMCHAIN_SECOND_CASHOUT_WINDOW           (60*60*24*30) /// 30 days
#define BMCHAIN_UPVOTE_LOCKOUT                  (fc::hours(12))

#define BMCHAIN_MIN_ACCOUNT_CREATION_FEE        0 /// 1 /// bmchain

#define BMCHAIN_OWNER_AUTH_RECOVERY_PERIOD                  fc::days(30)
#define BMCHAIN_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD  fc::days(1)
#define BMCHAIN_OWNER_UPDATE_LIMIT                          fc::minutes(60)
#define BMCHAIN_OWNER_AUTH_HISTORY_TRACKING_START_BLOCK_NUM 3186477

#endif

#define BMCHAIN_BLOCK_INTERVAL                  3
#define BMCHAIN_BLOCKS_PER_YEAR                 (365*24*60*60/BMCHAIN_BLOCK_INTERVAL)
#define BMCHAIN_BLOCKS_PER_DAY                  (24*60*60/BMCHAIN_BLOCK_INTERVAL)
#define BMCHAIN_START_MINER_VOTING_BLOCK        (BMCHAIN_BLOCKS_PER_DAY * 30)

#define BMCHAIN_INIT_MINER_NAME                 "initminer"
#define BMCHAIN_NUM_INIT_MINERS                 1
#define BMCHAIN_INIT_TIME                       (fc::time_point_sec());

#define BMCHAIN_MAX_WITNESSES                   21

#define BMCHAIN_MAX_VOTED_WITNESSES             20
#define BMCHAIN_MAX_MINER_WITNESSES             0
#define BMCHAIN_MAX_RUNNER_WITNESSES            1

#define BMCHAIN_HARDFORK_REQUIRED_WITNESSES     17 // 17 of the 21 dpos witnesses (20 elected and 1 virtual time) required for hardfork. This guarantees 75% participation on all subsequent rounds.
#define BMCHAIN_MAX_TIME_UNTIL_EXPIRATION       (60*60) // seconds,  aka: 1 hour
#define BMCHAIN_MAX_MEMO_SIZE                   2048
#define BMCHAIN_MAX_PROXY_RECURSION_DEPTH       4
#define BMCHAIN_SAVINGS_WITHDRAW_TIME        	(fc::days(3))
#define BMCHAIN_SAVINGS_WITHDRAW_REQUEST_LIMIT  100
#define BMCHAIN_VOTE_REGENERATION_SECONDS       (5*60*60*24) // 5 day
#define BMCHAIN_MAX_VOTE_CHANGES                5
#define BMCHAIN_REVERSE_AUCTION_WINDOW_SECONDS  (60*30) /// 30 minutes
#define BMCHAIN_MIN_VOTE_INTERVAL_SEC           (3)
#define BMCHAIN_VOTE_DUST_THRESHOLD             (10000) /// bmchain stress-testing, было 50000000

#define BMCHAIN_MIN_ROOT_COMMENT_INTERVAL       (fc::seconds(60*5)) // 5 minutes
#define BMCHAIN_MIN_REPLY_INTERVAL              (fc::seconds(20)) // 20 seconds

#define BMCHAIN_MAX_ACCOUNT_WITNESS_VOTES       30

#define BMCHAIN_100_PERCENT                     10000
#define BMCHAIN_1_PERCENT                       (BMCHAIN_100_PERCENT/100)
#define BMCHAIN_1_TENTH_PERCENT                 (BMCHAIN_100_PERCENT/1000)

#define BMCHAIN_CONTENT_REWARD_PERCENT          (90*BMCHAIN_1_PERCENT)

#define BMCHAIN_BANDWIDTH_AVERAGE_WINDOW_SECONDS (60*60*24*7) ///< 1 week
#define BMCHAIN_BANDWIDTH_PRECISION             (uint64_t(1000000)) ///< 1 million
#define BMCHAIN_MAX_COMMENT_DEPTH               0xffff // 64k
#define BMCHAIN_SOFT_MAX_COMMENT_DEPTH          0xff // 255

#define BMCHAIN_MAX_RESERVE_RATIO               (20000)

#define BMCHAIN_CREATE_ACCOUNT_WITH_BMT_MODIFIER 30
#define BMCHAIN_CREATE_ACCOUNT_DELEGATION_RATIO  5
#define BMCHAIN_CREATE_ACCOUNT_DELEGATION_TIME   fc::days(30)

#define BMCHAIN_MINING_REWARD                   asset( 1000, BMT_SYMBOL )
#define BMCHAIN_EQUIHASH_N                      140
#define BMCHAIN_EQUIHASH_K                      6

#define BMCHAIN_LIQUIDITY_REWARD_PERIOD_SEC     (60*60)
#define BMCHAIN_LIQUIDITY_REWARD_BLOCKS         (BMCHAIN_LIQUIDITY_REWARD_PERIOD_SEC/BMCHAIN_BLOCK_INTERVAL)
#define BMCHAIN_MIN_LIQUIDITY_REWARD            (asset( 1000*BMCHAIN_LIQUIDITY_REWARD_BLOCKS, BMT_SYMBOL )) // Minumum reward to be paid out to liquidity providers
#define BMCHAIN_MIN_CONTENT_REWARD              BMCHAIN_MINING_REWARD
#define BMCHAIN_MIN_CURATE_REWARD               BMCHAIN_MINING_REWARD
#define BMCHAIN_MIN_PRODUCER_REWARD             BMCHAIN_MINING_REWARD
#define BMCHAIN_MIN_POW_REWARD                  BMCHAIN_MINING_REWARD

#define BMCHAIN_ACTIVE_CHALLENGE_FEE            asset( 2000, BMT_SYMBOL )
#define BMCHAIN_OWNER_CHALLENGE_FEE             asset( 30000, BMT_SYMBOL )
#define BMCHAIN_ACTIVE_CHALLENGE_COOLDOWN       fc::days(1)
#define BMCHAIN_OWNER_CHALLENGE_COOLDOWN        fc::days(1)

#define BMCHAIN_POST_REWARD_FUND_NAME           ("post")
#define BMCHAIN_COMMENT_REWARD_FUND_NAME        ("comment")
#define BMCHAIN_RECENT_RSHARES_DECAY_RATE       (fc::days(15))
#define BMCHAIN_CONTENT_CONSTANT                (uint128_t(uint64_t(2000000000000ll)))
// note, if redefining these constants make sure calculate_claims doesn't overflow

// 5ccc e802 de5f
// int(expm1( log1p( 1 ) / BLOCKS_PER_YEAR ) * 2**BMCHAIN_APR_PERCENT_SHIFT_PER_BLOCK / 100000 + 0.5)
// we use 100000 here instead of 10000 because we end up creating an additional 9x for vesting
#define BMCHAIN_APR_PERCENT_MULTIPLY_PER_BLOCK          ( (uint64_t( 0x5ccc ) << 0x20) \
                                                        | (uint64_t( 0xe802 ) << 0x10) \
                                                        | (uint64_t( 0xde5f )        ) \
                                                        )
// chosen to be the maximal value such that BMCHAIN_APR_PERCENT_MULTIPLY_PER_BLOCK * 2**64 * 100000 < 2**128
#define BMCHAIN_APR_PERCENT_SHIFT_PER_BLOCK             87

#define BMCHAIN_APR_PERCENT_MULTIPLY_PER_ROUND          ( (uint64_t( 0x79cc ) << 0x20 ) \
                                                        | (uint64_t( 0xf5c7 ) << 0x10 ) \
                                                        | (uint64_t( 0x3480 )         ) \
                                                        )

#define BMCHAIN_APR_PERCENT_SHIFT_PER_ROUND             83

// We have different constants for hourly rewards
// i.e. hex(int(math.expm1( math.log1p( 1 ) / HOURS_PER_YEAR ) * 2**BMCHAIN_APR_PERCENT_SHIFT_PER_HOUR / 100000 + 0.5))
#define BMCHAIN_APR_PERCENT_MULTIPLY_PER_HOUR           ( (uint64_t( 0x6cc1 ) << 0x20) \
                                                        | (uint64_t( 0x39a1 ) << 0x10) \
                                                        | (uint64_t( 0x5cbd )        ) \
                                                        )

// chosen to be the maximal value such that BMCHAIN_APR_PERCENT_MULTIPLY_PER_HOUR * 2**64 * 100000 < 2**128
#define BMCHAIN_APR_PERCENT_SHIFT_PER_HOUR              77

// These constants add up to GRAPHENE_100_PERCENT.  Each GRAPHENE_1_PERCENT is equivalent to 1% per year APY
// *including the corresponding 9x vesting rewards*
#define BMCHAIN_CURATE_APR_PERCENT              3875
#define BMCHAIN_CONTENT_APR_PERCENT             3875
#define BMCHAIN_LIQUIDITY_APR_PERCENT            750
#define BMCHAIN_PRODUCER_APR_PERCENT             750
#define BMCHAIN_POW_APR_PERCENT                  750

#define BMCHAIN_MIN_PAYOUT                      (asset(20, BMT_SYMBOL))

#define BMCHAIN_MIN_ACCOUNT_NAME_LENGTH          3
#define BMCHAIN_MAX_ACCOUNT_NAME_LENGTH         16

#define BMCHAIN_MIN_PERMLINK_LENGTH             0
#define BMCHAIN_MAX_PERMLINK_LENGTH             256
#define BMCHAIN_MAX_WITNESS_URL_LENGTH          2048

#define BMCHAIN_INIT_SUPPLY                     int64_t(91907667000ll)
#define BMCHAIN_MAX_SHARE_SUPPLY                int64_t(1000000000000000ll)
#define BMCHAIN_MAX_SIG_CHECK_DEPTH             2

#define BMCHAIN_MIN_TRANSACTION_SIZE_LIMIT      1024
#define BMCHAIN_SECONDS_PER_YEAR                (uint64_t(60*60*24*365ll))

#define BMCHAIN_MAX_TRANSACTION_SIZE            (1024*64)
#define BMCHAIN_MIN_BLOCK_SIZE_LIMIT            (BMCHAIN_MAX_TRANSACTION_SIZE)
#define BMCHAIN_MAX_BLOCK_SIZE                  (BMCHAIN_MAX_TRANSACTION_SIZE*BMCHAIN_BLOCK_INTERVAL*2000)
#define BMCHAIN_MIN_BLOCK_SIZE                  115
#define BMCHAIN_BLOCKS_PER_HOUR                 (60*60/BMCHAIN_BLOCK_INTERVAL)
#define BMCHAIN_FEED_INTERVAL_BLOCKS            (BMCHAIN_BLOCKS_PER_HOUR) /// bmchain
#define BMCHAIN_FEED_HISTORY_WINDOW             (12*7) // 3.5 days
#define BMCHAIN_MAX_FEED_AGE_SECONDS            (60*60*24*7) // 7 days
#define BMCHAIN_MIN_FEEDS                       (BMCHAIN_MAX_WITNESSES/BMCHAIN_MAX_WITNESSES) /// bmchain /// protects the network from conversions before price has been established
#define BMCHAIN_CONVERSION_DELAY                (fc::hours(BMCHAIN_FEED_HISTORY_WINDOW)) //3.5 day conversion

#define BMCHAIN_MIN_UNDO_HISTORY                10
#define BMCHAIN_MAX_UNDO_HISTORY                10000

#define BMCHAIN_MIN_TRANSACTION_EXPIRATION_LIMIT (BMCHAIN_BLOCK_INTERVAL * 5) // 5 transactions per block
#define BMCHAIN_BLOCKCHAIN_PRECISION            uint64_t( 1000 )

#define BMCHAIN_BLOCKCHAIN_PRECISION_DIGITS     3
#define BMCHAIN_MAX_INSTANCE_ID                 (uint64_t(-1)>>16)
/** NOTE: making this a power of 2 (say 2^15) would greatly accelerate fee calcs */
#define BMCHAIN_MAX_AUTHORITY_MEMBERSHIP        10
#define BMCHAIN_MAX_ASSET_WHITELIST_AUTHORITIES 10
#define BMCHAIN_MAX_URL_LENGTH                  127

#define BMCHAIN_IRREVERSIBLE_THRESHOLD          (50 * BMCHAIN_1_PERCENT)

#define VIRTUAL_SCHEDULE_LAP_LENGTH  ( fc::uint128(uint64_t(-1)) )
#define VIRTUAL_SCHEDULE_LAP_LENGTH2 ( fc::uint128::max_value() )

/**
 *  Reserved Account IDs with special meaning
 */
///@{
/// Represents the current witnesses
#define BMCHAIN_MINER_ACCOUNT                   "miners"
/// Represents the canonical account with NO authority (nobody can access funds in null account)
#define BMCHAIN_NULL_ACCOUNT                    "null"
/// Represents the canonical account with WILDCARD authority (anybody can access funds in temp account)
#define BMCHAIN_TEMP_ACCOUNT                    "temp"
/// Represents the canonical account for specifying you will vote for directly (as opposed to a proxy)
#define BMCHAIN_PROXY_TO_SELF_ACCOUNT           ""
/// Represents the canonical root post parent account
#define BMCHAIN_ROOT_POST_PARENT                (account_name_type())
///@}

#define BMCHAIN_ENABLE                          true
#define BMCHAIN_STRESS_TESTING                  false
#define BMCHAIN_FIRST_PAYOUT_BLOCK              ((20*60*24*7)) /// 7 days

#define BMCHAIN_VOTE_EMISSION_RATE              500
#define BMCHAIN_COMMENT_EMISSION_RATE           2000
#define BMCHAIN_POST_EMISSION_RATE              5000
#define BMCHAIN_USER_EMISSION_RATE              10000

#ifdef IS_TEST_NET
#define BMCHAIN_REVERSE_AUCTION_TIME (0)
#else
#define BMCHAIN_REVERSE_AUCTION_TIME (1467295200-(60*60*24*6))
#endif