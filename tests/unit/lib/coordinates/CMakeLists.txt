
target_sources(bsgalone-tests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/CommonViewport.cc
	${CMAKE_CURRENT_SOURCE_DIR}/CommonCoordinateFrame.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TopLeftViewportTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/CenteredViewportTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TopViewFrameTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/IsometricViewFrameTest.cc
	)

target_include_directories(bsgalone-tests PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
