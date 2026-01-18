
target_include_directories (net_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (net_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AsioContext.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsioServer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TcpServer.cc
	)
