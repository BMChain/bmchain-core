/*
 * Copyright (c) 2016 Steemit, Inc., and contributors.
 */
#pragma once

#define BMCHAIN_BLOCKCHAIN_VERSION              ( version(0, 0, 0) )
#define BMCHAIN_BLOCKCHAIN_HARDFORK_VERSION     ( hardfork_version( BMCHAIN_BLOCKCHAIN_VERSION ) )

#ifdef IS_TEST_NET
#define BMCHAIN_INIT_PRIVATE_KEY                (fc::ecc::private_key::regenerate(fc::sha256::hash(std::string("init_key"))))
#define BMCHAIN_INIT_PUBLIC_KEY_STR             (std::string( bmchain::protocol::public_key_type(BMCHAIN_INIT_PRIVATE_KEY.get_public_key()) ))
#define BMCHAIN_CHAIN_ID                        (fc::sha256::hash("testnet"))

#define VESTS_SYMBOL  (uint64_t(6) | (uint64_t('V') << 8) | (uint64_t('E') << 16) | (uint64_t('S') << 24) | (uint64_t('T') << 32) | (uint64_t('S') << 40)) ///< VESTS with 6 digits of precision
#define STEEM_SYMBOL  (uint64_t(3) | (uint64_t('T') << 8) | (uint64_t('E') << 16) | (uint64_t('S') << 24) | (uint64_t('T') << 32) | (uint64_t('S') << 40)) ///< STEEM with 3 digits of precision
#define SBD_SYMBOL    (uint64_t(3) | (uint64_t('T') << 8) | (uint64_t('B') << 16) | (uint64_t('D') << 24) ) ///< Test Backed Dollars with 3 digits of precision
#define STMD_SYMBOL   (uint64_t(3) | (uint64_t('T') << 8) | (uint64_t('S') << 16) | (uint64_t('T') << 24) | (uint64_t('D') << 32) ) ///< Test Dollars with 3 digits of precision

#define BMCHAIN_SYMBOL                          "TEST"
#define BMCHAIN_ADDRESS_PREFIX                  "TST"

#define BMCHAIN_GENESIS_TIME                    (fc::time_point_sec(1451606400))
#define BMCHAIN_MINING_TIME                     (fc::time_point_sec(1451606400))
#define BMCHAIN_CASHOUT_WINDOW_SECONDS          (60*10) /// 1 hr /// bmchain
#define BMCHAIN_CASHOUT_WINDOW_SECONDS_PRE_HF12 (BMCHAIN_CASHOUT_WINDOW_SECONDS)
#define BMCHAIN_CASHOUT_WINDOW_SECONDS_PRE_HF17 (BMCHAIN_CASHOUT_WINDOW_SECONDS)
#define BMCHAIN_SECOND_CASHOUT_WINDOW           (60*60*24*3) /// 3 days
#define BMCHAIN_MAX_CASHOUT_WINDOW_SECONDS      (60*60*24) /// 1 day
#define BMCHAIN_VOTE_CHANGE_LOCKOUT_PERIOD      (60*10) /// 10 minutes
#define BMCHAIN_UPVOTE_LOCKOUT_HF7              (fc::minutes(1))
#define BMCHAIN_UPVOTE_LOCKOUT_HF17             (fc::minutes(5))


#define BMCHAIN_ORIGINAL_MIN_ACCOUNT_CREATION_FEE 0
#define BMCHAIN_MIN_ACCOUNT_CREATION_FEE          0

#define BMCHAIN_OWNER_AUTH_RECOVERY_PERIOD                  fc::seconds(60)
#define BMCHAIN_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD  fc::seconds(12)
#define BMCHAIN_OWNER_UPDATE_LIMIT                          fc::seconds(0)
#define BMCHAIN_OWNER_AUTH_HISTORY_TRACKING_START_BLOCK_NUM 1
#else // IS LIVE STEEM NETWORK

#define BMCHAIN_INIT_PUBLIC_KEY_STR             "BMT76G5486bhLKXkXgN1nSkeDbpWEaiavF7i6tDULdpvyr69CLbHC"
#define BMCHAIN_CHAIN_ID                        (fc::sha256::hash("bmchain"))
#define VESTS_SYMBOL  (uint64_t(6) | (uint64_t('V') << 8) | (uint64_t('E') << 16) | (uint64_t('S') << 24) | (uint64_t('T') << 32) | (uint64_t('S') << 40)) ///< VESTS with 6 digits of precision
#define STEEM_SYMBOL  (uint64_t(3) | (uint64_t('B') << 8) | (uint64_t('M') << 16) | (uint64_t('T') << 24))  ///< STEEM with 3 digits of precision
#define SBD_SYMBOL    (uint64_t(3) | (uint64_t('S') << 8) | (uint64_t('B') << 16) | (uint64_t('D') << 24) ) ///< STEEM Backed Dollars with 3 digits of precision
#define STMD_SYMBOL   (uint64_t(3) | (uint64_t('S') << 8) | (uint64_t('T') << 16) | (uint64_t('M') << 24) | (uint64_t('D') << 32) ) ///< STEEM Dollars with 3 digits of precision
#define BMCHAIN_SYMBOL                          "BMT"
#define BMCHAIN_ADDRESS_PREFIX                  "BMT"

#define BMCHAIN_GENESIS_TIME                    (fc::time_point_sec(1458835200))
#define BMCHAIN_MINING_TIME                     (fc::time_point_sec(1458838800))
#define BMCHAIN_CASHOUT_WINDOW_SECONDS_PRE_HF12 (60*60*24)    /// 1 day
#define BMCHAIN_CASHOUT_WINDOW_SECONDS_PRE_HF17 (60*60*12)    /// 12 hours
#define BMCHAIN_CASHOUT_WINDOW_SECONDS          (60*60*24)    /// 24 hours, bmchain /// (60*60*24*7)  /// 7 days
#define BMCHAIN_SECOND_CASHOUT_WINDOW           (60*60*24*30) /// 30 days
#define BMCHAIN_MAX_CASHOUT_WINDOW_SECONDS      (60*60*24*14) /// 2 weeks
#define BMCHAIN_VOTE_CHANGE_LOCKOUT_PERIOD      (60*60*2)     /// 2 hours
#define BMCHAIN_UPVOTE_LOCKOUT_HF7              (fc::minutes(1))
#define BMCHAIN_UPVOTE_LOCKOUT_HF17             (fc::hours(1)) /// bmchain /// (fc::hours(12))

#define BMCHAIN_ORIGINAL_MIN_ACCOUNT_CREATION_FEE  100000
#define BMCHAIN_MIN_ACCOUNT_CREATION_FEE           0 /// 1 /// bmchain

#define BMCHAIN_OWNER_AUTH_RECOVERY_PERIOD                  fc::days(30)
#define BMCHAIN_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD  fc::days(1)
#define BMCHAIN_OWNER_UPDATE_LIMIT                          fc::minutes(60)
#define BMCHAIN_OWNER_AUTH_HISTORY_TRACKING_START_BLOCK_NUM 3186477

#endif

#define BMCHAIN_BLOCK_INTERVAL                  3
#define BMCHAIN_BLOCKS_PER_YEAR                 (365*24*60*60/BMCHAIN_BLOCK_INTERVAL)
#define BMCHAIN_BLOCKS_PER_DAY                  (24*60*60/BMCHAIN_BLOCK_INTERVAL)
#define BMCHAIN_START_VESTING_BLOCK             1 // (BMCHAIN_BLOCKS_PER_DAY * 7) // bmchain
#define BMCHAIN_START_MINER_VOTING_BLOCK        1200 // (BMCHAIN_BLOCKS_PER_DAY * 30)

#define BMCHAIN_INIT_MINER_NAME                 "initminer"
#define BMCHAIN_NUM_INIT_MINERS                 1
#define BMCHAIN_INIT_TIME                       (fc::time_point_sec());

#define BMCHAIN_MAX_WITNESSES                   21

#define BMCHAIN_MAX_VOTED_WITNESSES_HF0         19
#define BMCHAIN_MAX_MINER_WITNESSES_HF0         1
#define BMCHAIN_MAX_RUNNER_WITNESSES_HF0        1

#define BMCHAIN_MAX_VOTED_WITNESSES_HF17        20
#define BMCHAIN_MAX_MINER_WITNESSES_HF17        0
#define BMCHAIN_MAX_RUNNER_WITNESSES_HF17       1

#define BMCHAIN_HARDFORK_REQUIRED_WITNESSES     17 // 17 of the 21 dpos witnesses (20 elected and 1 virtual time) required for hardfork. This guarantees 75% participation on all subsequent rounds.
#define BMCHAIN_MAX_TIME_UNTIL_EXPIRATION       (60*60) // seconds,  aka: 1 hour
#define BMCHAIN_MAX_MEMO_SIZE                   2048
#define BMCHAIN_MAX_PROXY_RECURSION_DEPTH       4
#define BMCHAIN_VESTING_WITHDRAW_INTERVALS_PRE_HF_16 104
#define BMCHAIN_VESTING_WITHDRAW_INTERVALS      13
#define BMCHAIN_VESTING_WITHDRAW_INTERVAL_SECONDS (60*60*24*7) /// 1 week per interval
#define BMCHAIN_MAX_WITHDRAW_ROUTES             10
#define BMCHAIN_SAVINGS_WITHDRAW_TIME        	(fc::days(3))
#define BMCHAIN_SAVINGS_WITHDRAW_REQUEST_LIMIT  100
#define BMCHAIN_VOTE_REGENERATION_SECONDS       (5*60*60*24) // 5 day
#define BMCHAIN_MAX_VOTE_CHANGES                5
#define BMCHAIN_REVERSE_AUCTION_WINDOW_SECONDS  (60*30) /// 30 minutes
#define BMCHAIN_MIN_VOTE_INTERVAL_SEC           (0) /// bmchain stress-testing, было 3
#define BMCHAIN_VOTE_DUST_THRESHOLD             (10000) /// bmchain stress-testing, было 50000000

#define BMCHAIN_MIN_ROOT_COMMENT_INTERVAL       (fc::seconds(0)) // 5 minutes  /// bmchain
#define BMCHAIN_MIN_REPLY_INTERVAL              (fc::seconds(0)) // 20 seconds /// bmchain
#define BMCHAIN_POST_AVERAGE_WINDOW             (60*60*24u) // 1 day
#define BMCHAIN_POST_MAX_BANDWIDTH              (4*BMCHAIN_100_PERCENT) // 2 posts per 1 days, average 1 every 12 hours
#define BMCHAIN_POST_WEIGHT_CONSTANT            (uint64_t(BMCHAIN_POST_MAX_BANDWIDTH) * BMCHAIN_POST_MAX_BANDWIDTH)

#define BMCHAIN_MAX_ACCOUNT_WITNESS_VOTES       30

#define BMCHAIN_100_PERCENT                     10000
#define BMCHAIN_1_PERCENT                       (BMCHAIN_100_PERCENT/100)
#define BMCHAIN_1_TENTH_PERCENT                 (BMCHAIN_100_PERCENT/1000)
#define BMCHAIN_DEFAULT_SBD_INTEREST_RATE       (10*BMCHAIN_1_PERCENT) ///< 10% APR

#define BMCHAIN_INFLATION_RATE_START_PERCENT    (978) // Fixes block 7,000,000 to 9.5%
#define BMCHAIN_INFLATION_RATE_STOP_PERCENT     (95) // 0.95%
#define BMCHAIN_INFLATION_NARROWING_PERIOD      (250000) // Narrow 0.01% every 250k blocks
#define BMCHAIN_CONTENT_REWARD_PERCENT          (90*BMCHAIN_1_PERCENT) //75% of inflation, 7.125% inflation
#define BMCHAIN_VESTING_FUND_PERCENT            (0*BMCHAIN_1_PERCENT) //15% of inflation, 1.425% inflation

#define BMCHAIN_MINER_PAY_PERCENT               (BMCHAIN_1_PERCENT) // 1%
#define BMCHAIN_MIN_RATION                      100000
#define BMCHAIN_MAX_RATION_DECAY_RATE           (1000000)
#define BMCHAIN_FREE_TRANSACTIONS_WITH_NEW_ACCOUNT 100

#define BMCHAIN_BANDWIDTH_AVERAGE_WINDOW_SECONDS (60*60*24*7) ///< 1 week
#define BMCHAIN_BANDWIDTH_PRECISION             (uint64_t(1000000)) ///< 1 million
#define BMCHAIN_MAX_COMMENT_DEPTH_PRE_HF17      6
#define BMCHAIN_MAX_COMMENT_DEPTH               0xffff // 64k
#define BMCHAIN_SOFT_MAX_COMMENT_DEPTH          0xff // 255

#define BMCHAIN_MAX_RESERVE_RATIO               (20000)

#define BMCHAIN_CREATE_ACCOUNT_WITH_STEEM_MODIFIER 30
#define BMCHAIN_CREATE_ACCOUNT_DELEGATION_RATIO    5
#define BMCHAIN_CREATE_ACCOUNT_DELEGATION_TIME     fc::days(30)

#define BMCHAIN_MINING_REWARD                   asset( 1000, STEEM_SYMBOL )
#define BMCHAIN_EQUIHASH_N                      140
#define BMCHAIN_EQUIHASH_K                      6

#define BMCHAIN_LIQUIDITY_TIMEOUT_SEC           (fc::seconds(60*60*24*7)) // After one week volume is set to 0
#define BMCHAIN_MIN_LIQUIDITY_REWARD_PERIOD_SEC (fc::seconds(60)) // 1 minute required on books to receive volume
#define BMCHAIN_LIQUIDITY_REWARD_PERIOD_SEC     (60*60)
#define BMCHAIN_LIQUIDITY_REWARD_BLOCKS         (BMCHAIN_LIQUIDITY_REWARD_PERIOD_SEC/BMCHAIN_BLOCK_INTERVAL)
#define BMCHAIN_MIN_LIQUIDITY_REWARD            (asset( 1000*BMCHAIN_LIQUIDITY_REWARD_BLOCKS, STEEM_SYMBOL )) // Minumum reward to be paid out to liquidity providers
#define BMCHAIN_MIN_CONTENT_REWARD              BMCHAIN_MINING_REWARD
#define BMCHAIN_MIN_CURATE_REWARD               BMCHAIN_MINING_REWARD
#define BMCHAIN_MIN_PRODUCER_REWARD             BMCHAIN_MINING_REWARD
#define BMCHAIN_MIN_POW_REWARD                  BMCHAIN_MINING_REWARD

#define BMCHAIN_ACTIVE_CHALLENGE_FEE            asset( 2000, STEEM_SYMBOL )
#define BMCHAIN_OWNER_CHALLENGE_FEE             asset( 30000, STEEM_SYMBOL )
#define BMCHAIN_ACTIVE_CHALLENGE_COOLDOWN       fc::days(1)
#define BMCHAIN_OWNER_CHALLENGE_COOLDOWN        fc::days(1)

#define BMCHAIN_POST_REWARD_FUND_NAME           ("post")
#define BMCHAIN_COMMENT_REWARD_FUND_NAME        ("comment")
#define BMCHAIN_RECENT_RSHARES_DECAY_RATE_HF17  (fc::days(30))
#define BMCHAIN_RECENT_RSHARES_DECAY_RATE_HF19  (fc::days(15))
#define BMCHAIN_CONTENT_CONSTANT_HF0            (uint128_t(uint64_t(2000000000000ll)))
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

#define BMCHAIN_MIN_PAYOUT_SBD                  (asset(20,SBD_SYMBOL))

#define BMCHAIN_SBD_STOP_PERCENT                (5*BMCHAIN_1_PERCENT ) // Stop printing SBD at 5% Market Cap
#define BMCHAIN_SBD_START_PERCENT               (2*BMCHAIN_1_PERCENT) // Start reducing printing of SBD at 2% Market Cap

#define BMCHAIN_MIN_ACCOUNT_NAME_LENGTH          3
#define BMCHAIN_MAX_ACCOUNT_NAME_LENGTH         16

#define BMCHAIN_MIN_PERMLINK_LENGTH             0
#define BMCHAIN_MAX_PERMLINK_LENGTH             256
#define BMCHAIN_MAX_WITNESS_URL_LENGTH          2048

#define BMCHAIN_INIT_SUPPLY                     int64_t(200000000000ll)
#define BMCHAIN_MAX_SHARE_SUPPLY                int64_t(1000000000000000ll)
#define STEEMIT_MAX_SIG_CHECK_DEPTH             2

#define BMCHAIN_MIN_TRANSACTION_SIZE_LIMIT      1024
#define BMCHAIN_SECONDS_PER_YEAR                (uint64_t(60*60*24*365ll))

#define BMCHAIN_SBD_INTEREST_COMPOUND_INTERVAL_SEC  (60*60*24*30)
#define BMCHAIN_MAX_TRANSACTION_SIZE            (1024*64)
#define BMCHAIN_MIN_BLOCK_SIZE_LIMIT            (BMCHAIN_MAX_TRANSACTION_SIZE)
#define BMCHAIN_MAX_BLOCK_SIZE                  (BMCHAIN_MAX_TRANSACTION_SIZE*BMCHAIN_BLOCK_INTERVAL*2000)
#define BMCHAIN_MIN_BLOCK_SIZE                  115
#define BMCHAIN_BLOCKS_PER_HOUR                 (60*60/BMCHAIN_BLOCK_INTERVAL)
#define BMCHAIN_FEED_INTERVAL_BLOCKS            (BMCHAIN_BLOCKS_PER_HOUR / 24) /// bmchain
#define BMCHAIN_FEED_HISTORY_WINDOW_PRE_HF_16   (24*7) /// 7 days * 24 hours per day
#define BMCHAIN_FEED_HISTORY_WINDOW             (12*7) // 3.5 days
#define BMCHAIN_MAX_FEED_AGE_SECONDS            (60*60*24*7) // 7 days
#define BMCHAIN_MIN_FEEDS                       (BMCHAIN_MAX_WITNESSES/BMCHAIN_MAX_WITNESSES) /// bmchain /// protects the network from conversions before price has been established
#define BMCHAIN_CONVERSION_DELAY_PRE_HF_16      (fc::days(7))
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

#define BMCHAIN_IRREVERSIBLE_THRESHOLD          (75 * BMCHAIN_1_PERCENT)

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
#define BMCHAIN_FIRST_PAYOUT_BLOCK              ((20*60*24*2)) /// 2 days

#define BMCHAIN_INIT_HARDFORK                   0        /// Hardfork to apply when creating a new chain
#define BMCHAIN_VOTE_EMISSION_RATE              50
#define BMCHAIN_COMMENT_EMISSION_RATE           200
#define BMCHAIN_POST_EMISSION_RATE              500
#define BMCHAIN_USER_EMISSION_RATE              1000

#define STEEMIT_HF_19_RECENT_CLAIMS (fc::uint128_t(uint64_t(140797515942543623ull)))
#define STEEMIT_HF_17_RECENT_CLAIMS (fc::uint128_t(808638359297ull,13744269167557038121ull)) // 14916744862149894120447332012073
#define STEEMIT_HF_17_NUM_POSTS     (49357)
#define STEEMIT_HF_17_NUM_REPLIES   (242051)
#define STEEMIT_MIN_LIQUIDITY_REWARD_PERIOD_SEC_HF10 fc::seconds(60*30) /// 30 min

#ifdef IS_TEST_NET
#define STEEMIT_HARDFORK_0_6_REVERSE_AUCTION_TIME (0)
#else
#define STEEMIT_HARDFORK_0_6_REVERSE_AUCTION_TIME (1467295200-(60*60*24*6))
#endif