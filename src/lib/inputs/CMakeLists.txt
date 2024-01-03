
target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Motion.cc
	${CMAKE_CURRENT_SOURCE_DIR}/IInputHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractInputHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameScreenInputHandler.cc
	)
