
target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/common
	)

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IScreenHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SpriteRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScreenCommon.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginScreenHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameScreenUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameScreenSystemHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameScreenShipHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameScreenHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MapScreenHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostScreenHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameOverScreenHandler.cc
	)
