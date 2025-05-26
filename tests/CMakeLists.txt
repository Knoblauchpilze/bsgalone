
find_package (Eigen3 3.4 REQUIRED NO_MODULE)
find_package (GTest REQUIRED)
find_package (libpqxx REQUIRED)

add_executable (bsgalone_tests)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/unit
	)

target_sources (bsgalone_tests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (bsgalone_tests PUBLIC
	${GTEST_INCLUDE_DIRS}
	)

target_link_libraries(bsgalone_tests
	core_utils
	bsgalone_core_lib
	pge_lib
	Eigen3::Eigen
	${GTEST_LIBRARIES}
	pq
	pqxx
	)
