
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Viewport.cc
	${CMAKE_CURRENT_SOURCE_DIR}/CenteredViewport.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TopLeftViewport.cc
	${CMAKE_CURRENT_SOURCE_DIR}/CoordinateFrame.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TransformedViewFrame.cc
	${CMAKE_CURRENT_SOURCE_DIR}/IsometricViewFrame.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TopViewFrame.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
