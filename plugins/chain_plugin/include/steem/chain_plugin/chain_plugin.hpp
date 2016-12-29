#pragma once
#include <appbase/application.hpp>
#include <steemit/chain/database.hpp>

namespace steem {
   using steemit::chain::database;
   using namespace appbase;

   class chain_plugin : public plugin<chain_plugin>
   {
      public:
        APPBASE_PLUGIN_REQUIRES();
        virtual void set_program_options( options_description& cli, options_description& cfg ) override;

        void plugin_initialize( const variables_map& options );
        void plugin_startup();
        void plugin_shutdown();

        database& db() { return _db; }

      private:
        database _db;
   };

}
