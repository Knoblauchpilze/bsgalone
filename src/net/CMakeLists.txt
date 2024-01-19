
add_library (bsgalone_net_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/connection
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/server
	)

target_link_libraries (bsgalone_net_lib
	)

target_include_directories (bsgalone_net_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	/usr/local/include/asio
	)
