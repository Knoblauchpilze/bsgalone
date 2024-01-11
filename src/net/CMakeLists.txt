
add_library (bsgalone_net_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_sources (bsgalone_net_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/MessageType.cc
	)

target_link_libraries (bsgalone_net_lib
	)

target_include_directories (bsgalone_net_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
