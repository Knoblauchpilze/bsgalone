
target_include_directories (game_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/input
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/internal
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/system
	)

target_sources (game_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/SystemProcessorAdapter.cc
	)
