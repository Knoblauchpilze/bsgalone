
add_library (client_new_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_sources (client_new_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/App.cc
	)

target_link_libraries (client_new_lib
	core_lib
	net_lib
	time_lib
	pge_lib
	)

target_include_directories (client_new_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
