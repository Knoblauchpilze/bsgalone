
add_executable (integrationTests)

target_include_directories (integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/helpers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/bsgo
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/net
	)

target_sources (integrationTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_link_libraries (integrationTests
	GTest::gtest
	bsgalone-interface-test-library
	core_lib
	bsgo_lib
	net_lib
	)
