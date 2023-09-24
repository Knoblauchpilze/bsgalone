
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/CoordinateFrame.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TransformedViewFrame.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
