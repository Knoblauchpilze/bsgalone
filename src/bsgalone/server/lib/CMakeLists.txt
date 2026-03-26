
add_library (bsgalone_server_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (bsgalone_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Server.cc
	)

target_link_libraries (bsgalone_server_lib
	core_lib
	)
