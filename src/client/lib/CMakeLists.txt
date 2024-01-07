
add_library (bsgalone_client_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

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
	core_utils
	pge_app_lib
	bsgalone_core_lib
	)

target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
