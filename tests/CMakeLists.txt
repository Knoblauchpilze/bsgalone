
find_package (Eigen3 3.4 REQUIRED NO_MODULE)
find_package (GTest REQUIRED)
find_package (libpqxx REQUIRED)

add_executable(bsgalone-tests)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/unit
	)

target_sources (bsgalone-tests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	)

target_include_directories (bsgalone-tests PUBLIC
	${GTEST_INCLUDE_DIRS}
	)

target_link_libraries(bsgalone-tests
	core_utils
	bsgalone_lib
	Eigen3::Eigen
	${GTEST_LIBRARIES}
	pq
	pqxx
	)
