
target_include_directories (game_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/common
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/game
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/outpost
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/login
	)

target_sources (game_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingScreenUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MapScreenUiHandler.cc
	)
