
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/client
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/core
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/server
	)
