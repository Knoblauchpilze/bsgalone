
add_library (bsgalone_core_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/domain
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/messages
	)

target_link_libraries (bsgalone_core_lib PUBLIC
	bsgalone-interface-library
	net_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (bsgalone_core_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
