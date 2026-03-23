
add_library (tests_helper_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (tests_helper_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (tests_helper_lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Comparison.cc
	)

target_link_libraries (tests_helper_lib PUBLIC
	bsgalone-interface-library
  bsgalone_core_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (tests_helper_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
