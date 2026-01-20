
target_include_directories(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/AsioContextTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsioServerTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TcpServerTest.cc
	)
