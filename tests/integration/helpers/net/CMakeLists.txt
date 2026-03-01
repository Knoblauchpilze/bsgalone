
target_include_directories(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/ConnectedSockets.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SocketUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TcpAcceptor.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TestNetworkEventQueue.cc
	)
