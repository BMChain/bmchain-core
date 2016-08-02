file(GLOB HEADERS "include/steemit/plugins/rank/*.hpp")

add_library( steemit_rank
             ${HEADERS}
             rank_plugin.cpp
             rank_api.cpp
           )

target_link_libraries( steemit_rank steemit_app steemit_chain fc graphene_db )
target_include_directories( steemit_rank
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
