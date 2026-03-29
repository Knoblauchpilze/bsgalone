
target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/commands
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/network
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/queues
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/serialization
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/types
	)
