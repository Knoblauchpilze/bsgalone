
target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/data
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/events
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/repositories
	)
