
add_library (client_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_sources (client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/App.cc
	)

target_link_libraries (client_lib
	core_lib
	pge_lib
	bsgo_lib
	time_lib
	ui_lib
	game_core_lib
	game_client_lib
	)

target_include_directories (client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
