
target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/models
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/ports
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/usecases
	)
