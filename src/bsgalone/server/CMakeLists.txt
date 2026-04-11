
add_executable (bsgalone_server)

target_include_directories (bsgalone_server PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	)

target_sources (bsgalone_server PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_link_libraries (bsgalone_server PUBLIC
	bsgalone-interface-library
	core_lib
	bsgalone_server_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (bsgalone_server PUBLIC
		bsgalone-interface-test-library
		)
endif ()
