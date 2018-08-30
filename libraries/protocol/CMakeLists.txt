file(GLOB HEADERS "include/bmchain/protocol/*.hpp")

## SORT .cpp by most likely to change / break compile
add_library( steemit_protocol

             types.cpp
             authority.cpp
             operations.cpp
             sign_state.cpp
             operation_util_impl.cpp
             bmchain_operations.cpp
             transaction.cpp
             block.cpp
             asset.cpp
             version.cpp
             get_config.cpp

             ${HEADERS}
        include/bmchain/protocol/legacy_asset.hpp include/bmchain/protocol/custom_token_operations.hpp custom_token_operations.cpp)

target_link_libraries( steemit_protocol fc graphene_utilities )
target_include_directories( steemit_protocol
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include" )

INSTALL( TARGETS
   steemit_protocol

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)

INSTALL( FILES ${HEADERS} DESTINATION "include/bmchain/protocol" )
