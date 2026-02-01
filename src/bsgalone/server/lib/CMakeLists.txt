
add_library (game_server_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_sources (game_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/GameServer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ServerEventListener.cc
	)

target_link_libraries (game_server_lib
	core_lib
	net_lib
	)

target_include_directories (game_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
