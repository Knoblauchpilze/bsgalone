
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/bsgo
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/pge
	)

target_include_directories(bsgalone_tests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
