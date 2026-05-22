
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/game
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/messages
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/network
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/store
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/ui
	)
