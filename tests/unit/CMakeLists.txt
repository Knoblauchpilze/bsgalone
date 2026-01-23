
add_executable (unitTests)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/bsgo
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/client
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/helpers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/net
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/pge
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/server
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/time
	)

target_sources (unitTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (unitTests
	GTest::gtest
	GTest::gmock
	bsgalone-interface-test-library
	core_lib
	bsgo_lib
	pge_lib
	time_lib
	client_lib
	server_lib
	)
