
target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/GameScreenUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponsUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbilitiesUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameOverUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityUiHandler.cc
	)
