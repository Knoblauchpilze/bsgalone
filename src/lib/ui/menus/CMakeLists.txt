
target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Action.cc
	${CMAKE_CURRENT_SOURCE_DIR}/BackgroundDesc.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MenuContentDesc.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Menu.cc

	${CMAKE_CURRENT_SOURCE_DIR}/MenuCommon.cc
	${CMAKE_CURRENT_SOURCE_DIR}/BackgroundConfig.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TextConfig.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MenuConfig.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UiMenu.cc
	)
