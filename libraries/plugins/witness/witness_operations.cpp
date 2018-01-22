#include <bmchain/witness/witness_operations.hpp>

#include <bmchain/protocol/operation_util_impl.hpp>

namespace bmchain { namespace witness {

void enable_content_editing_operation::validate()const
{
   chain::validate_account_name( account );
}

} } // bmchain::witness

DEFINE_OPERATION_TYPE( bmchain::witness::witness_plugin_operation )
