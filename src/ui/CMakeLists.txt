
add_library (ui_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (ui_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (ui_lib PRIVATE
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

target_link_libraries (ui_lib PUBLIC
	bsgalone-interface-library
	pge_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (ui_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
