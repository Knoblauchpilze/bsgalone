
target_include_directories (bsgalone_client_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/BackgroundConfig.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MenuCommon.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TextConfig.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UiBlinkingMenu.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UiMenu.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UiPictureMenu.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UiTextField.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UiTextMenu.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UiTimedMenu.cc
	)
