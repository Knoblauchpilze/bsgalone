
add_library (bsgalone_client_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/consumers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/enums
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/game
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/inputs
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/network
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/renderers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/ui
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/views
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/App.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameApp.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameClient.cc
	)

target_link_libraries (bsgalone_client_lib
	core_lib
	net_lib
	pge_lib
	ui_lib
	bsgo_lib
	game_core_lib
	)

target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
