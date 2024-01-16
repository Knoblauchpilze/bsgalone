
target_include_directories(bsgalone_tests PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources(bsgalone_tests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/MessageTypeTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UuidTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/StringTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OptionalTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DockMessageTest.cc
	)
