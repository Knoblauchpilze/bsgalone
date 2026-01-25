
add_executable (game_client)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	)

target_sources (game_client PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (game_client PUBLIC
	)

target_link_libraries (game_client PUBLIC
	bsgalone-interface-library
	core_lib
	game_client_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (game_client PUBLIC
		bsgalone-interface-test-library
		)
endif ()
