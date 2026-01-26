
target_include_directories(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/PortFixture.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TcpFixture.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TcpServerFixture.cc
	)
