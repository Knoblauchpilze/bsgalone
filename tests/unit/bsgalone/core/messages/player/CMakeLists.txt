
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityMessageTest.cc
	)
