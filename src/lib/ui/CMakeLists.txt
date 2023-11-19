
target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/menus
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

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MapScreenUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginScreenUiHandler.cc
	)
