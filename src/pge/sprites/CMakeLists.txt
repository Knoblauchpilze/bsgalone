
target_sources (pge_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/DecalResource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TexturePack.cc
	)

target_include_directories (pge_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
