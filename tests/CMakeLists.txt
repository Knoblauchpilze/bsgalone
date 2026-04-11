
find_package(GTest REQUIRED)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/unit
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/integration
	)
