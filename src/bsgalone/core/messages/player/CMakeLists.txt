
target_include_directories (game_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (game_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/DockMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpRequestedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityMessage.cc
	)
