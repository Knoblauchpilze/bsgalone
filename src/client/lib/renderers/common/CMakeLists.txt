
target_include_directories (bsgalone_client_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/EntitiesUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DecalScreenRenderer.cc
	)
