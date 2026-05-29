
add_library(tests_helper_lib SHARED "")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories(tests_helper_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/bsgalone
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/messaging
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/net
	)

target_link_libraries(tests_helper_lib PUBLIC
	bsgalone-interface-library
	pge_lib
	net_lib
	bsgalone_core_lib
	bsgalone_client_lib
	bsgalone_server_lib
	)

if(${ENABLE_TESTS})
	target_link_libraries(tests_helper_lib PUBLIC
		bsgalone-interface-test-library
		)
endif()
