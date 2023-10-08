
target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/TiledBackground.cc
	${CMAKE_CURRENT_SOURCE_DIR}/StringUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntitiesUtils.cc
	)
