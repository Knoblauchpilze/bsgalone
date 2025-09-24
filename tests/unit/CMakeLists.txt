
add_executable (unitTests)

target_sources (unitTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/bsgo
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/pge
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/time
	)

target_include_directories (unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (unitTests
	GTest::gtest_main
	bsgalone-interface-test-library
	core_lib
	bsgo_lib
	pge_lib
	time_lib
	)
