project (bsgalone_client_lib)

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library (bsgalone_client_lib SHARED "")

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/app
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/coordinates
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/game
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/inputs
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/renderers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/ui
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/App.cc
	)

target_link_libraries (bsgalone_client_lib
	png
	X11
	GL
	pthread
	core_utils
	bsgalone_core_lib
	)

target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/app
	${CMAKE_CURRENT_SOURCE_DIR}/coordinates
	${CMAKE_CURRENT_SOURCE_DIR}/game
	${CMAKE_CURRENT_SOURCE_DIR}/inputs
	${CMAKE_CURRENT_SOURCE_DIR}/renderers
	${CMAKE_CURRENT_SOURCE_DIR}/ui
	)
