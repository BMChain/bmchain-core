#include <bmchain/protocol/custom_token_operations.hpp>
#include <fc/io/json.hpp>

#include <locale>

namespace bmchain { namespace protocol {

   void custom_token_create_operation::validate() const {

   }

   void custom_token_transfer_operation::validate() const {

   }

   void custom_token_setup_emissions_operation::validate() const {
      FC_ASSERT( schedule_time > BMCHAIN_GENESIS_TIME );
   }

   void custom_token_set_setup_parameters_operation::validate() const {

   }

}}