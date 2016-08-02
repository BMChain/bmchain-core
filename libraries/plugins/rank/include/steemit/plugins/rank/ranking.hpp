
#pragma once

#include <fc/reflect/reflect.hpp>
#include <fc/variant.hpp>

#include <vector>

namespace steemit { namespace plugin { namespace rank {

struct query
{
   fc::variant filter;
   fc::variant start;
   uint32_t limit = 0;
};

class ranking
{
   public:
      virtual void get_objects( std::vector< fc::variant >& result, const query& q ) = 0;
};

} } }

FC_REFLECT( steemit::plugin::rank::query, (filter)(start)(limit) )
