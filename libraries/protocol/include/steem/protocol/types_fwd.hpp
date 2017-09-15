#pragma once

namespace fc {
class uint128;
class variant;
} // fc

namespace steem { namespace protocol {
template< typename Storage = fc::uint128 >
class fixed_string;

class asset_symbol_type;
} } // steem::protocol

namespace fc { namespace raw {

template<typename Stream>
inline void pack( Stream& s, const uint128& u );
template<typename Stream>
inline void unpack( Stream& s, uint128& u );

template< typename Stream, typename Storage >
inline void pack( Stream& s, const steem::protocol::fixed_string< Storage >& u );
template< typename Stream, typename Storage >
inline void unpack( Stream& s, steem::protocol::fixed_string< Storage >& u );

template< typename Stream >
inline void pack( Stream& s, const steem::protocol::asset_symbol_type& sym );
template< typename Stream >
inline void unpack( Stream& s, steem::protocol::asset_symbol_type& sym );

} // raw

template< typename Storage >
inline void to_variant( const steem::protocol::fixed_string< Storage >& s, fc::variant& v );
template< typename Storage >
inline void from_variant( const variant& v, steem::protocol::fixed_string< Storage >& s );
inline void to_variant( const steem::protocol::asset_symbol_type& sym, fc::variant& v );

} // fc
