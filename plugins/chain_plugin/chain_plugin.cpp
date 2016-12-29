#include <steem/chain_plugin/chain_plugin.hpp>

namespace steem {

void chain_plugin::set_program_options( options_description& cli, options_description& cfg ) 
{
   cfg.add_options()
         ("readonly", "open the database in read only mode")
         ("dbsize", bpo::value<uint64_t>()->default_value( 8*1024 ), "Minimum size MB of database shared memory file")
         ;
   cli.add_options()
         ("replay", "clear chain database and replay all blocks" )
         ("reset", "clear chain database and block log" )
         ;
}

void chain_plugin::plugin_initialize( const variables_map& options ) {


}

void chain_plugin::plugin_startup() {

}

void chain_plugin::plugin_shutdown() {
}

}
