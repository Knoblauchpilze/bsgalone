
add_executable (unitTests)

target_include_directories (unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/bsgalone
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/core
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/helpers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/messaging
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/net
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/pge
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/time
	)

target_sources (unitTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_link_libraries (unitTests
	GTest::gtest
	GTest::gmock
	bsgalone-interface-test-library
	core_lib
	pge_lib
	time_lib
	net_lib
	bsgalone_core_lib
	bsgalone_client_lib
	)
