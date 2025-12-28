
target_include_directories (net_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (net_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractEventBus.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EventProcessor.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SynchronizedEventBus.cc
	)

