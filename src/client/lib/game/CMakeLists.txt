
target_sources (client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Screen.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Game.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameMessageModule.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameSession.cc
	)

target_include_directories (client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
