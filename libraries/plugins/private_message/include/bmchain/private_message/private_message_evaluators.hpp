#pragma once

#include <bmchain/chain/evaluator.hpp>

#include <bmchain/private_message/private_message_operations.hpp>
#include <bmchain/private_message/private_message_plugin.hpp>

namespace bmchain { namespace private_message {

DEFINE_PLUGIN_EVALUATOR( private_message_plugin, bmchain::private_message::private_message_plugin_operation, private_message )

} }
