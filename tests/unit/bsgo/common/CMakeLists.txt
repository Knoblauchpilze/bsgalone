
target_include_directories(bsgalone_tests PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources(bsgalone_tests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/VectorUtilsTest.cc
	)
