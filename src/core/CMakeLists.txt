
add_library (core_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_sources (core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/CoreException.cc
	${CMAKE_CURRENT_SOURCE_DIR}/CoreObject.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Environment.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SafetyNet.cc
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/log
	)

target_include_directories (core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (core_lib PUBLIC
	bsgalone-interface-library
	)

if (${ENABLE_TESTS})
	target_link_libraries (core_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
