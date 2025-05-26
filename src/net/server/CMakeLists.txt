
target_include_directories (bsgalone_net_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_net_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/TcpServer.cc
	)

