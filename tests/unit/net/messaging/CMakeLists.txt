
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/AsyncEventBusTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EventProcessorTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SynchronizedEventBusTest.cc
	)
