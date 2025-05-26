
add_library (bsgalone_server_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/clients
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/consumers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/game
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/queues
	)

target_sources (bsgalone_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Server.cc
	)

target_link_libraries (bsgalone_server_lib
	core_utils
	bsgalone_net_lib
	bsgo_lib
	)

target_include_directories (bsgalone_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
