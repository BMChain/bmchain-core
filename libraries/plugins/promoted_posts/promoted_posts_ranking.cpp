
#include <steemit/plugins/promoted_posts/promoted_posts_ranking.hpp>
#include <steemit/plugins/promoted_posts/promoted_post_object.hpp>

#include <steemit/chain/database.hpp>

// TODO: Only need this for get_discussion, which should be moved to own plugin #210
#include <steemit/app/database_api.hpp>
#include <steemit/app/state.hpp>

namespace steemit { namespace plugin { namespace promoted_posts {

promoted_posts_ranking::promoted_posts_ranking( const chain::database& db )
   : _db(db) {}

void promoted_posts_ranking::get_objects( std::vector< fc::variant >& result, const rank::query& q )
{
   query::filter _filter;
   query::start _start;

   fc::from_variant( q.filter, _filter );
   fc::from_variant( q.start , _start  );

   const auto& by_tag_idx = _db.get_index_type< promoted_post_index >().indices().get< by_tag >();
   auto it = by_tag_idx.begin();
   if( _start.author.valid() && _start.permlink.valid() )
   {
      const chain::comment_object& comment = _db.get_comment( *_start.author, *_start.permlink );

      const auto& by_comment_id_idx = _db.get_index_type< promoted_post_index >().indices().get< by_comment_id >();
      auto cit = by_comment_id_idx.lower_bound( comment.id );
      // Iterate until we reach the given tag
      while( true )
      {
         if( cit == by_comment_id_idx.end() )
            return;
         if( cit->comment_id != comment.id )
            return;
         if( cit->tag == _filter.tag )
            break;
         ++cit;
      }

      // The given comment_id exists, so the first peer object should be for the empty tag
      it = by_tag_idx.iterator_to( *cit );
   }
   else
   {
      it = by_tag_idx.lower_bound( boost::make_tuple( _filter.tag ) );
   }

   while( true )
   {
      if( it == by_tag_idx.end() )
         return;
      if( it->tag != _filter.tag )
         return;
      result.emplace_back();
      steemit::app::discussion d = it->comment_id( _db );
      fill_discussion_info( _db, d, true );
      fc::to_variant( d, result.back() );
      ++it;
   }
}

} } }
