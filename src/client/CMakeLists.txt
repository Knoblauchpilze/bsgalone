
add_executable(bsgalone-client)

target_sources (bsgalone-client PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (bsgalone-client PUBLIC
	)

target_link_libraries(bsgalone-client
	core_utils
	bsgalone_lib
	)
