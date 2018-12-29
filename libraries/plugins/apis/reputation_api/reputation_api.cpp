#include <bmchain/plugins/reputation_api/reputation_api_plugin.hpp>
#include <bmchain/plugins/reputation_api/reputation_api.hpp>

#include <bmchain/plugins/reputation/reputation_objects.hpp>

namespace bmchain { namespace plugins { namespace reputation {

namespace detail {

    class reputation_api_impl
    {
    public:
        reputation_api_impl() : _db( appbase::app().get_plugin< bmchain::plugins::chain::chain_plugin >().db() ) {}

        DECLARE_API_IMPL(
        (get_account_reputations)
        )

        chain::database& _db;
    };

    DEFINE_API_IMPL( reputation_api_impl, get_account_reputations )
{
    FC_ASSERT( args.limit <= 1000, "Cannot retrieve more than 1000 account reputations at a time." );

    const auto& acc_idx = _db.get_index< chain::account_index >().indices().get< chain::by_name >();
    const auto& rep_idx = _db.get_index< reputation::reputation_index >().indices().get< reputation::by_account >();

    auto acc_itr = acc_idx.lower_bound( args.account_lower_bound );

    get_account_reputations_return result;
    result.reputations.reserve( args.limit );


return result;
}

} // detail

reputation_api::reputation_api(): my( new detail::reputation_api_impl() )
{
    JSON_RPC_REGISTER_API( STEEM_REPUTATION_API_PLUGIN_NAME );
}

reputation_api::~reputation_api() {}

DEFINE_READ_APIS( reputation_api,
(get_account_reputations)
)

} } } // bmchain::plugins::reputation