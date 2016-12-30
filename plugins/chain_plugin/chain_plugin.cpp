#include <steem/chain_plugin/chain_plugin.hpp>

namespace steem {

class chain_plugin_impl {
   public:
      uint64_t  shared_memory_size = 0;
      bfs::path shared_memory_dir;
      bfs::path block_log_dir;
      bool      replay = false;
      bool      reset   = false;
      bool      readonly = false;

      database  db;
};


chain_plugin::chain_plugin()
:my( new chain_plugin_impl() ) {
}

chain_plugin::~chain_plugin(){}

database& chain_plugin::db() { return my->db; }

void chain_plugin::set_program_options( options_description& cli, options_description& cfg ) 
{
   cfg.add_options()
         ("readonly", bpo::value<bool>()->default_value(false), "open the database in read only mode")
         ("shared-file-dir", bpo::value<bfs::path>()->default_value("blockchain/"), 
            "the location of the chain shared memory files (absolute path or relative to application data dir)")
         ("shared-file-size", bpo::value<uint64_t>()->default_value( 8*1024 ), 
            "Minimum size MB of database shared memory file")
         ("blocklog", bpo::value<bfs::path>()->default_value("blockchain/"),
            "the directory containing blocklog.bin and blocklog.index files (absolute or relative to application data dir)")
         ;
   cli.add_options()
         ("replay", bpo::value<bool>()->default_value(false), 
          "clear chain database and replay all blocks" )
         ("reset", bpo::value<bool>()->default_value(false), 
          "clear chain database and block log" )
         ;
}

void chain_plugin::plugin_initialize( const variables_map& options ) {
  my->shared_memory_dir = app().data_dir() / "blockchain"; 
  my->block_log_dir = app().data_dir() / "blockchain"; 

  if( options.count( "shared-file-dir" ) ) {
    auto sfd = options.at("shared-file-dir").as<bfs::path>();
    if( sfd.is_relative() )
       my->shared_memory_dir = app().data_dir() / sfd;
    else
       my->shared_memory_dir = sfd;
  }
  if( options.count( "block-log-dir" ) ) {
    auto bld = options.at("block-log-dir").as<bfs::path>();
    if( bld.is_relative() )
       my->block_log_dir = app().data_dir() / bld;
    else
       my->block_log_dir = bld;
  }
  my->readonly = options.count( "readonly" ) > 0;
  my->replay   = options.count( "replay" ) > 0;
  my->reset    = options.count( "reset" ) > 0;
}

void chain_plugin::plugin_startup() {
  if( !my->readonly ) {
     if( my->reset ) {

     } else if( my->replay ) {

     }
  }
}

void chain_plugin::plugin_shutdown() {
}

}
