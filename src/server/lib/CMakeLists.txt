
add_library (server_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/consumers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/game
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/network
	)

target_sources (server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Server.cc
	)

target_link_libraries (server_lib
	core_lib
	net_lib
	bsgo_lib
	time_lib
	game_core_lib
	game_server_lib
	)

target_include_directories (server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
