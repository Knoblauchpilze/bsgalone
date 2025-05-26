
target_sources (pge_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/RenderingPass.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TiledBackground.cc
	)

target_include_directories (pge_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
