
target_include_directories(bsgalone_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/consumers
	)

target_sources(bsgalone_server_lib PRIVATE
	)
