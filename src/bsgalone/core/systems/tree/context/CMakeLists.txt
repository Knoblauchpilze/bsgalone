
target_include_directories (game_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (game_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ContextKey.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DataContext.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DataKey.cc
	)
