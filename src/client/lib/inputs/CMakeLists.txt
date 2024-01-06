
target_include_directories (bsgalone_client_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Motion.cc
	${CMAKE_CURRENT_SOURCE_DIR}/IInputHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameScreenInputHandler.cc
	)
