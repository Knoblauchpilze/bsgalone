
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/DataReader.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DataSender.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SocketConnector.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TestTcpServer.cc
	)
