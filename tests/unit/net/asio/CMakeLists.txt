
target_sources(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/AsioNetFixture.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ReadingSocketTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WritingSocketTest.cc
	)

target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
