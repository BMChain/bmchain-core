#include <steem/chain_plugin/chain_plugin.hpp>
#include <steemit/chain/database_exceptions.hpp>
#include <fc/io/json.hpp>

namespace steem {

using namespace steemit;
using fc::flat_map;
using chain::block_id_type;

class chain_plugin_impl {
   public:
      uint64_t                         shared_memory_size = 0;
      bfs::path                        shared_memory_dir;
      bfs::path                        block_log_dir;
      bool                             replay = false;
      bool                             reset   = false;
      bool                             readonly = false;
      uint32_t                         flush_interval = 0;
      flat_map<uint32_t,block_id_type> loaded_checkpoints;

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
         ("shared-file-dir", bpo::value<bfs::path>()->default_value("blockchain"), 
            "the location of the chain shared memory files (absolute path or relative to application data dir)")
         ("shared-file-size", bpo::value<uint64_t>()->default_value( 8*1024 ), 
            "Minimum size MB of database shared memory file")
         ("blocklog", bpo::value<bfs::path>()->default_value("blockchain"),
            "the directory containing blocklog.bin and blocklog.index files (absolute or relative to application data dir)")
         ("checkpoint,c", bpo::value<vector<string>>()->composing(), "Pairs of [BLOCK_NUM,BLOCK_ID] that should be enforced as checkpoints.")
         ("flush-state-interval", bpo::value<uint32_t>()->default_value(0), 
          "flush shared memory changes to disk every N blocks" )
         ;
   cli.add_options()
         ("replay-blockchain", bpo::value<bool>()->default_value(false), 
          "clear chain database and replay all blocks" )
         ("resync-blockchain", bpo::value<bool>()->default_value(false), 
          "clear chain database and block log" )
         ;
}

void chain_plugin::plugin_initialize( const variables_map& options ) {
  ilog( "initializing chain plugin" );
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
  my->shared_memory_size = options.at( "shared-file-size" ).as<uint64_t>() * 1024 * 1024;
  my->readonly = options.at( "readonly" ).as<bool>();
  my->replay   = options.at( "replay-blockchain" ).as<bool>();
  my->reset    = options.at( "resync-blockchain" ).as<bool>();
  my->flush_interval = options.at( "flush-state-interval" ).as<uint32_t>();

  if( options.count("checkpoint") )
  {
     auto cps = options.at("checkpoint").as<vector<string>>();
     my->loaded_checkpoints.reserve( cps.size() );
     for( auto cp : cps )
     {
        auto item = fc::json::from_string(cp).as<std::pair<uint32_t,block_id_type> >();
        my->loaded_checkpoints[item.first] = item.second;
     }
  }
}

void chain_plugin::plugin_startup() {
  if( !my->readonly ) {
     ilog( "starting chain in read/write mode" );
     if( my->reset ) {
        wlog( "resync requested: deleting block log and shared memory" );
        my->db.wipe( my->block_log_dir, my->shared_memory_dir, true );
     } else if( my->replay ) {
        wlog( "replay requested: deleting shared memory" );
        my->db.wipe( my->block_log_dir, my->shared_memory_dir, false);
     }
     my->db.set_flush_interval( my->flush_interval );
     my->db.add_checkpoints( my->loaded_checkpoints );

     if( my->replay ) {
        ilog("Replaying blockchain on user request.");
        my->db.reindex( my->block_log_dir, my->shared_memory_dir, my->shared_memory_size );
     } else {
        try {
           ilog( "opening block log from ${path}", ("path",my->block_log_dir.generic_string()) );
           ilog( "opening shared memory from ${path}", ("path",my->shared_memory_dir.generic_string()) );
           my->db.open( my->block_log_dir, my->shared_memory_dir, 
                        0,
                        my->shared_memory_size,
                        chainbase::database::read_write );
        } catch ( const fc::assert_exception& e ) {
           try {
              wlog( "Error opening database, attempting to replay blockchain" );
              my->db.reindex( my->block_log_dir, my->shared_memory_dir, my->shared_memory_size );
           } catch ( chain::block_log_exception& ) {
              wlog( "Error opening block log, resync required" );
           }
        }
     }  
  } else {
      ilog( "Starting chain in read mode." );
      my->db.open( my->block_log_dir, my->shared_memory_dir, 0, my->shared_memory_size, chainbase::database::read_only );
  }
}

void chain_plugin::plugin_shutdown() {
   ilog( "closing chain database" );
   my->db.close();
   ilog( "database closed successfully" );
}

}
