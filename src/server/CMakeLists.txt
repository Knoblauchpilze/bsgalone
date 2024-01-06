
add_executable(bsgalone-server)

target_sources (bsgalone-server PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (bsgalone-server PUBLIC
	)

target_link_libraries(bsgalone-server
	core_utils
	bsgalone_lib
	)
