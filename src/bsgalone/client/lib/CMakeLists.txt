
add_library (game_client_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_sources (game_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/GameApp.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameClient.cc
	)

target_link_libraries (game_client_lib
	core_lib
	net_lib
	pge_lib
	)

target_include_directories (game_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
