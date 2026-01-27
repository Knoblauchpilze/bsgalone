
add_library (game_core_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/network
	)

target_include_directories (game_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (game_core_lib PUBLIC
	bsgalone-interface-library
	bsgo_lib
	net_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (game_core_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
