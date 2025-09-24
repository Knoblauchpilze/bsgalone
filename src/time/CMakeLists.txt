
add_library (time_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_sources (time_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Duration.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TimeManager.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TimeStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Unit.cc
	)

target_include_directories (time_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (time_lib PUBLIC
	bsgalone-interface-library
	core_lib
	bsgo_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (time_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
