
target_include_directories (game_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (game_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractGameEventQueue.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameEventProcessor.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SynchronizedGameEventQueue.cc
	)
