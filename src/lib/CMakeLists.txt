
add_library (bsgalone_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package (libpqxx REQUIRED)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/bsgo
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/coordinates
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/app
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/game
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/ui
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/renderers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/inputs
	)

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/App.cc
	)

target_link_libraries (bsgalone_lib
	png
	X11
	GL
	pthread
	stdc++fs
	pq
	pqxx
	)

target_include_directories (bsgalone_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/app
	${CMAKE_CURRENT_SOURCE_DIR}/bsgo
	${CMAKE_CURRENT_SOURCE_DIR}/coordinates
	${CMAKE_CURRENT_SOURCE_DIR}/game
	${CMAKE_CURRENT_SOURCE_DIR}/views
	${CMAKE_CURRENT_SOURCE_DIR}/renderers
	${CMAKE_CURRENT_SOURCE_DIR}/ui
	)
