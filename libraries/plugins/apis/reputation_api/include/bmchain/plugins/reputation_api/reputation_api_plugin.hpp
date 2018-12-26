#pragma once
#include <steem/plugins/json_rpc/utility.hpp>
#include <steem/plugins/reputation/reputation_objects.hpp>
#include <steem/plugins/database_api/database_api_objects.hpp>

#include <steem/protocol/types.hpp>

#include <fc/optional.hpp>
#include <fc/variant.hpp>
#include <fc/vector.hpp>

namespace bmchain { namespace plugins { namespace reputation {

using bmchain::protocol::account_name_type;

namespace detail
{
    class reputation_api_impl;
}

struct account_reputation
{
    account_name_type account;
    bmchain::protocol::share_type   reputation;
};

struct get_account_reputations_args
{
    account_name_type account_lower_bound;
    uint32_t          limit = 1000;
};

} } } // bmchain::plugins::reputation

FC_REFLECT( bmchain::plugins::reputation::account_reputation,
            (account)(reputation) );

FC_REFLECT( bmchain::plugins::reputation::get_account_reputations_args,
            (account_lower_bound)(limit) );

FC_REFLECT( bmchain::plugins::reputation::get_account_reputations_return,
            (reputations) );