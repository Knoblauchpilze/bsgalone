
target_include_directories (bsgalone_client_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/GameScreenUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponsUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbilitiesUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameOverUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/StatusUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipStatusUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogUiHandler.cc
	)
