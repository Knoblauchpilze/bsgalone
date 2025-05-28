
add_executable (integrationTests)

target_sources (integrationTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (integrationTests
	GTest::gtest_main
	bsgalone-interface-test-library
	core_lib
	)
