
#pragma once

#include <steemit/app/plugin.hpp>

#include <boost/container/flat_map.hpp>

namespace steemit { namespace chain {
class database;
struct signed_block;
} }

namespace steemit { namespace app {
class application;
} }

namespace steemit { namespace plugin { namespace rank {

class ranking;

class rank_plugin : public steemit::app::plugin
{
   public:
      rank_plugin();
      virtual ~rank_plugin();

      virtual std::string plugin_name()const override;
      virtual void plugin_initialize( const boost::program_options::variables_map& options ) override;
      virtual void plugin_startup() override;
      virtual void plugin_shutdown() override;

      void register_ranking( const std::string& name, std::shared_ptr< ranking > r );

      boost::container::flat_map< std::string, std::shared_ptr< ranking > > _ranking_map;
};

} } }
