#pragma once

#include <bmchain/chain/util/asset.hpp>
#include <bmchain/chain/bmchain_objects.hpp>

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
   uint128_t  total_reward_shares2;
   asset      total_reward_fund_bmt;
   price      current_bmt_price;
   curve_id   reward_curve = quadratic;
   uint128_t  content_constant = BMCHAIN_CONTENT_CONSTANT;
};

uint64_t get_rshare_reward( const comment_reward_context& ctx );

inline uint128_t get_content_constant_s()
{
   return BMCHAIN_CONTENT_CONSTANT; // looking good for posters
}

uint128_t evaluate_reward_curve( const uint128_t& rshares, const curve_id& curve = quadratic, const uint128_t& content_constant = BMCHAIN_CONTENT_CONSTANT );

inline bool is_comment_payout_dust( const price& p, uint64_t bmt_payout )
{
   return asset( bmt_payout, BMT_SYMBOL ) < BMCHAIN_MIN_PAYOUT;
}

} } } // bmchain::chain::util

FC_REFLECT( bmchain::chain::util::comment_reward_context,
   (rshares)
   (reward_weight)
   (total_reward_shares2)
   (total_reward_fund_bmt)
   (current_bmt_price)
   (reward_curve)
   (content_constant)
   )
