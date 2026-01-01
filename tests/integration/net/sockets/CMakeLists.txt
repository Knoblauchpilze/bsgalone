
target_include_directories(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/ReadingSocketTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TcpServerFixture.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WritingSocketTest.cc
	)
