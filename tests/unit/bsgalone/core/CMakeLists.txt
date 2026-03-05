
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/domain
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/enums
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/messages
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/network
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/systems
	)
