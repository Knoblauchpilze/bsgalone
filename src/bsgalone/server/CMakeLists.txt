
add_executable (game_server)

target_sources (game_server PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (game_server PUBLIC
	)

target_link_libraries (game_server PUBLIC
	bsgalone-interface-library
	core_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (game_server PUBLIC
		bsgalone-interface-test-library
		)
endif ()
