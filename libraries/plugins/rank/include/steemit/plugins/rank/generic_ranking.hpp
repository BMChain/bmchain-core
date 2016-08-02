
namespace graphene { namespace chain {
class operation;
} }

namespace steemit { namespace plugin { namespace rank {

// -Drank_promoted_object_type=123
// DEFINE_RANK_OBJECT("promoted", comment_object, author_permlink, share_type)

#define DEFINE_RANK_OBJECT( Name, ObjectType, ObjectIdentifier, RankKeyType )                \
                                                                                             \
class rank_ ## Name ## _object : public abstract_object< rank_ ## Name ## _object >          \
{                                                                                            \
   static const uint8_t space_id = RANK_SPACE_ID;                                            \
   static const uint8_t type_id = rank_ ## Name ## _object_type;                             \
                                                                                             \
   ObjectIdentifier identifier;                                                              \
   RankKeyType      rank_key;                                                                \
};                                                                                           \
                                                                                             \
typedef multi_index_container<
   rank_ ## Name ## _object,
   indexed_by<
      ordered_unique< tag< by_id >, member< object, object_id_type, &object::id > >,
      ordered_unique< tag< by_rank_identifier >,
         composite_key< rank_ ## Name ## _object,
            member< rank_ ## Name ## _object, RankKeyType, &rank_ ## Name ## _object::rank_key >,
            member< rank_ ## Name ## _object, ObjectIdentifier, &rank_ ## Name ## _object::identifier >
         >,
         composite_key_compare< std::less< RankKeyType >, std::less< ObjectIdentifier >
      >,
      ordered_unique< tag< by_identifier >,
         member< rank_ ## Name ## _object, ObjectIdentifier, &rank_ ## Name ## _object::identifier >
      >
   >
> rank_ ## Name ## _multi_index_type;

typedef graphene::db::generic_index< rank_ ## Name ## _object, rank_ ## Name ## _multi_index_type> rank_ ## Name ## _index;

FC_REFLECT_DERIVED( steemit::plugin::rank::rank_ ## Name ## _object, (graphene::db::object), (identifier)(rank_key) );


template<
   typename ObjectType,
   typename ObjectIdentifier,
   typename RankKeyType
   >
class generic_ranking : public ranking
{
   public:
      virtual bool get_rank_key( const ObjectType& obj, RankKeyType& key ) = 0;
      virtual void flag_dirty( const operation& op, std::function<void(const ObjectIdentifier&)> cb ) = 0;
      virtual void register_index()
      {
         FC_ASSERT( _app != nullptr );
         _app->chain_database()->add_index< primary_index< index >
      }

      virtual void update_ranking( const signed_block& b ) = 0;

      virtual void set_app( app::application& a )
      {
         _app = a;
      }

      virtual void get_by_rank( const lower_bound_id& id )
      {
         _app
      }

      app::application* _app = nullptr;

};

} } }
