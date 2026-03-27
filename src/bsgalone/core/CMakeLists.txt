
add_library (bsgalone_core_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/domain
	)

target_link_libraries (bsgalone_core_lib PUBLIC
	bsgalone-interface-library
	)

if (${ENABLE_TESTS})
	target_link_libraries (bsgalone_core_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
