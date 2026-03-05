
target_include_directories (game_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/processes
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/services
	)

target_sources (game_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/MessageExchanger.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemProcessor.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemProcessorUtils.cc
	)
