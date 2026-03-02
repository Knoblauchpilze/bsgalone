
target_include_directories (game_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/components
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/entities
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/enums
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/events
	)

target_sources (game_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Target.cc
	)
