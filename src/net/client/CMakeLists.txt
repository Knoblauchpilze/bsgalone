
target_include_directories (net_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (net_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AsioClient.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ConnectionStatus.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TcpClient.cc
	)
