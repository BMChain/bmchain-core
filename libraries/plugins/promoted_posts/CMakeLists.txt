file(GLOB HEADERS "include/steemit/plugins/promoted_posts/*.hpp")

add_library( steemit_promoted_posts
             ${HEADERS}
             promoted_posts_plugin.cpp
             promoted_posts_api.cpp
             promoted_posts_ranking.cpp
           )

target_link_libraries( steemit_promoted_posts steemit_rank steemit_app steemit_chain fc graphene_db )
target_include_directories( steemit_promoted_posts
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
