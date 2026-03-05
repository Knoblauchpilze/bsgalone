
target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/adapters
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/app
	)
