#pragma once

#include <bmchain/chain/util/asset.hpp>
#include <bmchain/chain/steem_objects.hpp>

#include <bmchain/protocol/asset.hpp>
#include <bmchain/protocol/config.hpp>
#include <bmchain/protocol/types.hpp>

#include <fc/reflect/reflect.hpp>

#include <fc/uint128.hpp>

namespace bmchain { namespace chain { namespace util {

using bmchain::protocol::asset;
using bmchain::protocol::price;
using bmchain::protocol::share_type;

using fc::uint128_t;

struct comment_reward_context
{
   share_type rshares;
   uint16_t   reward_weight = 0;
   asset      max_sbd;
   uint128_t  total_reward_shares2;
   asset      total_reward_fund_steem;
   price      current_steem_price;
   curve_id   reward_curve = quadratic;
   uint128_t  content_constant = BMCHAIN_CONTENT_CONSTANT_HF0;
};

uint64_t get_rshare_reward( const comment_reward_context& ctx );

inline uint128_t get_content_constant_s()
{
   return BMCHAIN_CONTENT_CONSTANT_HF0; // looking good for posters
}

uint128_t evaluate_reward_curve( const uint128_t& rshares, const curve_id& curve = quadratic, const uint128_t& content_constant = BMCHAIN_CONTENT_CONSTANT_HF0 );

inline bool is_comment_payout_dust( const price& p, uint64_t steem_payout )
{
   return to_sbd( p, asset( steem_payout, BMT_SYMBOL ) ) < BMCHAIN_MIN_PAYOUT_SBD;
}

} } } // bmchain::chain::util

FC_REFLECT( bmchain::chain::util::comment_reward_context,
   (rshares)
   (reward_weight)
   (max_sbd)
   (total_reward_shares2)
   (total_reward_fund_steem)
   (current_steem_price)
   (reward_curve)
   (content_constant)
   )
