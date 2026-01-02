
target_include_directories(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/server
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/sockets
	)

target_sources(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/TcpFixture.cc
	)
