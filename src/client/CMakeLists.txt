
add_executable(bsgalone)

target_sources (bsgalone PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (bsgalone PUBLIC
	)

target_link_libraries(bsgalone
	core_utils
	bsgalone_lib
	)
