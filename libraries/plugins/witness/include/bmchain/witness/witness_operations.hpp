#pragma once

#include <bmchain/protocol/base.hpp>
#include <bmchain/protocol/operation_util.hpp>

#include <bmchain/app/plugin.hpp>

namespace bmchain { namespace witness {

using namespace std;
using bmchain::protocol::base_operation;
using bmchain::chain::database;

class witness_plugin;

struct enable_content_editing_operation : base_operation
{
   protocol::account_name_type   account;
   fc::time_point_sec            relock_time;

   void validate()const;

   void get_required_active_authorities( flat_set< protocol::account_name_type>& a )const { a.insert( account ); }
};

typedef fc::static_variant<
         enable_content_editing_operation
      > witness_plugin_operation;

DEFINE_PLUGIN_EVALUATOR( witness_plugin, witness_plugin_operation, enable_content_editing );

} } // bmchain::witness

FC_REFLECT( bmchain::witness::enable_content_editing_operation, (account)(relock_time) )

FC_REFLECT_TYPENAME( bmchain::witness::witness_plugin_operation )

DECLARE_OPERATION_TYPE( bmchain::witness::witness_plugin_operation )
