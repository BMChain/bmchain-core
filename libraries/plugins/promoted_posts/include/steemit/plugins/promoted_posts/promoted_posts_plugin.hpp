
#pragma once

#include <steemit/app/plugin.hpp>

namespace steemit { namespace chain {
class operation_object;
} }

namespace steemit { namespace plugin { namespace promoted_posts {

class promoted_posts_ranking;

class promoted_posts_plugin : public steemit::app::plugin
{
   public:
      promoted_posts_plugin();
      virtual ~promoted_posts_plugin();

      virtual std::string plugin_name()const override;
      virtual void plugin_initialize( const boost::program_options::variables_map& options ) override;
      virtual void plugin_startup() override;
      virtual void plugin_shutdown() override;

      void on_post_apply_op( const chain::operation_object& op_obj );

      std::string                               _promotion_vendor;
      boost::signals2::scoped_connection        _post_apply_op_conn;
      std::shared_ptr< promoted_posts_ranking > _ranking;
};

} } }
