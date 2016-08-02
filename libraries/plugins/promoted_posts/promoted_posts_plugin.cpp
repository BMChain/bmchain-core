
#include <steemit/chain/database.hpp>
#include <steemit/chain/history_object.hpp>

#include <steemit/plugins/promoted_posts/promoted_posts_api.hpp>
#include <steemit/plugins/promoted_posts/promoted_posts_plugin.hpp>
#include <steemit/plugins/promoted_posts/promoted_posts_ranking.hpp>
#include <steemit/plugins/promoted_posts/promoted_post_object.hpp>

#include <steemit/plugins/rank/rank_plugin.hpp>

#include <string>

namespace steemit { namespace plugin { namespace promoted_posts {

promoted_posts_plugin::promoted_posts_plugin() {}
promoted_posts_plugin::~promoted_posts_plugin() {}

std::string promoted_posts_plugin::plugin_name()const
{
   return "promoted_posts";
}

void promoted_posts_plugin::plugin_initialize( const boost::program_options::variables_map& options )
{
}

void promoted_posts_plugin::plugin_startup()
{
   chain::database& db = database();
   app::application& ap = app();

   _ranking = std::make_shared< promoted_posts_ranking >( db );

   ap.register_api_factory< promoted_posts_api >( "promoted_posts_api" );
   std::shared_ptr< rank::rank_plugin > rplugin = ap.get_plugin< rank::rank_plugin >( "rank" );
   rplugin->register_ranking( "promoted_posts", _ranking );

   _post_apply_op_conn = db.post_apply_operation.connect([this](const chain::operation_object& op_obj){ on_post_apply_op( op_obj ); });
}

void promoted_posts_plugin::plugin_shutdown()
{
}

struct promote_memo
{
   std::string action;
   std::string post;
};

struct promotion_op_visitor
{
   typedef void result_type;

   promotion_op_visitor( chain::database& db, const std::string& promotion_vendor ) : _db(db), _promotion_vendor(promotion_vendor) {}

   template< typename Op >
   void operator()( const Op& op ) {}

   void operator()( const chain::transfer_operation& op )
   {
      FC_ASSERT( op.amount.symbol == SBD_SYMBOL );
      FC_ASSERT( op.to == _promotion_vendor );
      FC_ASSERT( op.memo.size() > 0 );
      FC_ASSERT( op.memo[0] == '{' );

      promote_memo m;
      fc::variant v = fc::json::from_string( op.memo );
      from_variant( v, m );

      FC_ASSERT( m.action == "promote" );
      FC_ASSERT( m.post.size() > 0 );

      size_t slash_pos = m.post.find( '/' );
      FC_ASSERT( slash_pos != std::string::npos );
      std::string author = m.post.substr( 0, slash_pos );
      std::string permlink = m.post.substr( slash_pos+1 );

      // throws if does not exist
      const chain::comment_object& comment = _db.get_comment( author, permlink );

      FC_ASSERT( comment.parent_author.size() == 0 );

      const auto& by_comment_id_idx = _db.get_index_type< promoted_post_index >().indices().get< by_comment_id >();
      auto it = by_comment_id_idx.find( comment.id );
      if( it == by_comment_id_idx.end() )
      {
         _db.create< promoted_post_object >( [&]( promoted_post_object& promo )
         {
            promo.comment_id = comment.id;
            promo.promotion_amount = op.amount.amount;
         } );
      }
      else
      {
         _db.modify< promoted_post_object >( *it, [&]( promoted_post_object& promo )
         {
            promo.comment_id = comment.id;
            promo.promotion_amount += op.amount.amount;
         } );
      }
   }

   chain::database&           _db;
   const std::string&         _promotion_vendor;
};

void promoted_posts_plugin::on_post_apply_op( const chain::operation_object& op_obj )
{
   chain::database& db = database();
   promotion_op_visitor vtor( db, _promotion_vendor );
   try
   {
      op_obj.op.visit( vtor );
   }
   catch( const fc::exception& e )
   {
      wlog( "Caught failed transfer to promotion vendor ${v} in block ${b}", ("v", _promotion_vendor)("b", db.head_block_num()) );
   }
}

} } } // steemit::plugin::promoted_posts

STEEMIT_DEFINE_PLUGIN( promoted_posts, steemit::plugin::promoted_posts::promoted_posts_plugin )

FC_REFLECT( steemit::plugin::promoted_posts::promote_memo, (action)(post) )
