#pragma once

#include <bmchain/protocol/asset.hpp>

#define STEEM_SYMBOL_LEGACY_SER_1   (uint64_t(1) | (STEEM_SYMBOL_U64 << 8))
#define STEEM_SYMBOL_LEGACY_SER_2   (uint64_t(2) | (STEEM_SYMBOL_U64 << 8))
#define STEEM_SYMBOL_LEGACY_SER_3   (uint64_t(5) | (STEEM_SYMBOL_U64 << 8))
#define STEEM_SYMBOL_LEGACY_SER_4   (uint64_t(3) | (uint64_t('0') << 8) | (uint64_t('.') << 16) | (uint64_t('0') << 24) | (uint64_t('0') << 32) | (uint64_t('1') << 40))
#define STEEM_SYMBOL_LEGACY_SER_5   (uint64_t(3) | (uint64_t('6') << 8) | (uint64_t('.') << 16) | (uint64_t('0') << 24) | (uint64_t('0') << 32) | (uint64_t('0') << 40))

namespace bmchain { namespace protocol {

        class legacy_steem_asset_symbol_type
        {
        public:
            legacy_steem_asset_symbol_type() {}

            bool is_canon()const
            {   return ( ser == STEEM_SYMBOL_SER );    }

            uint64_t ser = STEEM_SYMBOL_SER;
        };

        struct legacy_steem_asset
        {
        public:
            legacy_steem_asset() {}

            template< bool force_canon >
            asset to_asset()const
            {
                if( force_canon )
                {
                    FC_ASSERT( symbol.is_canon(), "Must use canonical BMT symbol serialization" );
                }
                return asset( amount, BMT_SYMBOL );
            }

            static legacy_steem_asset from_amount( share_type amount )
            {
                legacy_steem_asset leg;
                leg.amount = amount;
                return leg;
            }

            static legacy_steem_asset from_asset( const asset& a )
            {
                FC_ASSERT( a.symbol == BMT_SYMBOL );
                return from_amount( a.amount );
            }

            share_type                       amount;
            legacy_steem_asset_symbol_type   symbol;
        };

    } }

FC_REFLECT( bmchain::protocol::legacy_steem_asset_symbol_type,(ser))

namespace fc { namespace raw {

        template< typename Stream >
        inline void pack( Stream& s, const bmchain::protocol::legacy_steem_asset_symbol_type& sym )
        {
            switch( sym.ser )
            {
                case STEEM_SYMBOL_LEGACY_SER_1:
                case STEEM_SYMBOL_LEGACY_SER_2:
                case STEEM_SYMBOL_LEGACY_SER_3:
                case STEEM_SYMBOL_LEGACY_SER_4:
                case STEEM_SYMBOL_LEGACY_SER_5:
                    wlog( "pack legacy serialization ${s}", ("s", sym.ser) );
                case STEEM_SYMBOL_SER:
                    pack( s, sym.ser );
                    break;
                default:
                    FC_ASSERT( false, "Cannot serialize legacy symbol ${s}", ("s", sym.ser) );
            }
        }

        template< typename Stream >
        inline void unpack( Stream& s, bmchain::protocol::legacy_steem_asset_symbol_type& sym, uint32_t depth )
        {
            //  994240:        "account_creation_fee": "0.1 STEEM"
            // 1021529:        "account_creation_fee": "10.0 STEEM"
            // 3143833:        "account_creation_fee": "3.00000 STEEM"
            // 3208405:        "account_creation_fee": "2.00000 STEEM"
            // 3695672:        "account_creation_fee": "3.00 STEEM"
            // 4338089:        "account_creation_fee": "0.001 0.001"
            // 4626205:        "account_creation_fee": "6.000 6.000"
            // 4632595:        "account_creation_fee": "6.000 6.000"
            depth++;
            uint64_t ser = 0;

            fc::raw::unpack( s, ser, depth );
            switch( ser )
            {
                case STEEM_SYMBOL_LEGACY_SER_1:
                case STEEM_SYMBOL_LEGACY_SER_2:
                case STEEM_SYMBOL_LEGACY_SER_3:
                case STEEM_SYMBOL_LEGACY_SER_4:
                case STEEM_SYMBOL_LEGACY_SER_5:
                    wlog( "unpack legacy serialization ${s}", ("s", ser) );
                case STEEM_SYMBOL_SER:
                    sym.ser = ser;
                    break;
                default:
                    FC_ASSERT( false, "Cannot deserialize legacy symbol ${s}", ("s", ser) );
            }
        }

    } // fc::raw

    inline void to_variant( const bmchain::protocol::legacy_steem_asset& leg, fc::variant& v )
    {
        to_variant( leg.to_asset<false>(), v );
    }

    inline void from_variant( const fc::variant& v, bmchain::protocol::legacy_steem_asset& leg )
    {
        bmchain::protocol::asset a;
        from_variant( v, a );
        leg = bmchain::protocol::legacy_steem_asset::from_asset( a );
    }

//    template<>
//    struct get_typename< bmchain::protocol::legacy_steem_asset_symbol_type >
//    {
//        static const char* name()
//        {
//            return "bmchain::protocol::legacy_steem_asset_symbol_type";
//        }
//    };

} // fc



FC_REFLECT( bmchain::protocol::legacy_steem_asset,(amount)(symbol))
