
target_include_directories(tests_helper_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/client
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/core
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/server
	)

target_sources(tests_helper_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/TestDataFactory.cc
	)
