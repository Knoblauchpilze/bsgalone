
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SpriteRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScreenCommon.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameUiRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameSystemRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MapRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameOverRenderer.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
