
target_include_directories (game_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (game_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ClientEventListener.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ServerEventListener.cc
	)
