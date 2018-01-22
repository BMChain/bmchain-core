#include <bmchain/follow/follow_operations.hpp>

#include <bmchain/protocol/operation_util_impl.hpp>

namespace bmchain { namespace follow {

void follow_operation::validate()const
{
   FC_ASSERT( follower != following, "You cannot follow yourself" );
}

void reblog_operation::validate()const
{
   FC_ASSERT( account != author, "You cannot reblog your own content" );
}

} } //bmchain::follow

DEFINE_OPERATION_TYPE( bmchain::follow::follow_plugin_operation )
