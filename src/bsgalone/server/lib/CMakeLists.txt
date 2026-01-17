
add_library (server_new_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_sources (server_new_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Server.cc
	)

target_link_libraries (server_new_lib
	core_lib
	net_lib
	time_lib
	)

target_include_directories (server_new_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
