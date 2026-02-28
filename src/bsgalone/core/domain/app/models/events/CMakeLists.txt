
target_include_directories (game_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (game_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/EntityDiedEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameEventType.cc
	${CMAKE_CURRENT_SOURCE_DIR}/IGameEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpCompletedEvent.cc
	)
