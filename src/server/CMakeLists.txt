
add_executable (bsgalone_server)

target_sources (bsgalone_server PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (bsgalone_server PUBLIC
	)

target_link_libraries(bsgalone_server
	core_utils
	bsgalone_core_lib
	)
