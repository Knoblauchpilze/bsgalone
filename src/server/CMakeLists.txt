
add_executable (bsgalone_server)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	)

target_sources (bsgalone_server PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (bsgalone_server PUBLIC
	)

target_link_libraries(bsgalone_server
	core_utils
	bsgalone_net_lib
	bsgalone_server_lib
	bsgo_lib
	)
