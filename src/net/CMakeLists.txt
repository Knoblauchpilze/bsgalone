
add_library (net_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/connection
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/server
	)

target_include_directories (net_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (net_lib PUBLIC
	bsgalone-interface-library
	)

if (${ENABLE_TESTS})
	target_link_libraries (net_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
