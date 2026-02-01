
add_library (client_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/consumers
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

target_sources (client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/App.cc
	)

target_link_libraries (client_lib
	core_lib
	pge_lib
	bsgo_lib
	time_lib
	game_core_lib
	)

target_include_directories (client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
