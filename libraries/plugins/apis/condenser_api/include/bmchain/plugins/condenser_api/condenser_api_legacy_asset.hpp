#pragma once

#include <bmchain/protocol/asset.hpp>

namespace bmchain { namespace plugins { namespace condenser_api {

    using bmchain::protocol::asset;
    using bmchain::protocol::asset_symbol_type;
    using bmchain::protocol::share_type;

    struct legacy_asset {
    public:
        legacy_asset() {}

        asset to_asset() const {
            return asset(amount, symbol);
        }

        operator asset() const { return to_asset(); }

        static legacy_asset from_asset(const asset &a) {
            legacy_asset leg;
            leg.amount = a.amount;
            leg.symbol = a.symbol;
            return leg;
        }

        std::string to_string() const;

        static legacy_asset from_string(const std::string &from);

        share_type amount;
        asset_symbol_type symbol = BMT_SYMBOL;
    };

} } } // bmchain::plugins::condenser_api

namespace fc {

    inline void to_variant( const bmchain::plugins::condenser_api::legacy_asset& a, fc::variant& var )
    {
        var = a.to_string();
    }

    inline void from_variant( const fc::variant& var, bmchain::plugins::condenser_api::legacy_asset& a )
    {
        a = bmchain::plugins::condenser_api::legacy_asset::from_string( var.as_string() );
    }

} // fc

FC_REFLECT( bmchain::plugins::condenser_api::legacy_asset,(amount)(symbol))
