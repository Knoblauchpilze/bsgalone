
target_include_directories (server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/services
	)

target_sources (server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/MessageConsumerSetup.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemProcessorUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemProcessor.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MessageExchanger.cc
	)

