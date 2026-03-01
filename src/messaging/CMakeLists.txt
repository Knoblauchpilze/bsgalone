
# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries
add_library (messaging_lib INTERFACE "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (messaging_lib INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (messaging_lib INTERFACE
	bsgalone-interface-library
	core_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (messaging_lib INTERFACE
		bsgalone-interface-test-library
		)
endif ()
