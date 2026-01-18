
add_executable (bsgalone_client_new)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	)

target_sources (bsgalone_client_new PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (bsgalone_client_new PUBLIC
	)

target_link_libraries (bsgalone_client_new PUBLIC
	bsgalone-interface-library
	core_lib
	net_lib
	client_new_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (bsgalone_client_new PUBLIC
		bsgalone-interface-test-library
		)
endif ()
