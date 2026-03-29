
target_include_directories (bsgalone_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/driven
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/driving
	)
