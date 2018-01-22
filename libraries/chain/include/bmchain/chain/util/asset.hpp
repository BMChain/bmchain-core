#pragma once

#include <bmchain/protocol/asset.hpp>

namespace bmchain { namespace chain { namespace util {

using bmchain::protocol::asset;
using bmchain::protocol::price;

inline asset to_sbd( const price& p, const asset& steem )
{
   FC_ASSERT( steem.symbol == BMT_SYMBOL );
   if( p.is_null() )
      return asset( 0, SBD_SYMBOL );
   return steem * p;
}

inline asset to_steem( const price& p, const asset& sbd )
{
   FC_ASSERT( sbd.symbol == SBD_SYMBOL );
   if( p.is_null() )
      return asset( 0, BMT_SYMBOL );
   return sbd * p;
}

} } }
