
target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/game
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/login
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Palette.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScreenCommon.cc
	)
