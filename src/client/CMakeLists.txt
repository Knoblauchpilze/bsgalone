
add_executable (bsgalone_client)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	)

target_sources (bsgalone_client PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (bsgalone_client PUBLIC
	)

target_link_libraries (bsgalone_client PUBLIC
	bsgalone-interface-library
	core_lib
	client_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (bsgalone_client PUBLIC
		bsgalone-interface-test-library
		)
endif ()
