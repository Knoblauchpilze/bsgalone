
target_include_directories (server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/input
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/internal
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/system
	)

target_sources (server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/SystemMessageConsumerSetup.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TriageMessageConsumer.cc
	)
