
target_include_directories(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/client
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/server
	)
