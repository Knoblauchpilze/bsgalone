
add_executable (bsgalone_server)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	)

target_sources (bsgalone_server PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (bsgalone_server PUBLIC
	)

target_link_libraries (bsgalone_server PUBLIC
	bsgalone-interface-library
	core_utils
	net_lib
	bsgo_lib
	server_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (bsgalone_server PUBLIC
		bsgalone-interface-test-library
		)
endif ()
