
add_library (time_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

# https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions
target_compile_definitions (time_lib PUBLIC
	-DHAS_REMOTE_API=0
	)

target_sources (time_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/TimeSerialization.cc
	${CMAKE_CURRENT_SOURCE_DIR}/tz.cpp
	)

target_include_directories (time_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (time_lib PUBLIC
	bsgalone-interface-library
	core_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (time_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
