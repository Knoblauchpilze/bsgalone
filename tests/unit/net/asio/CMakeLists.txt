
target_sources(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/ReadingSocketTest.cc
	)

target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
