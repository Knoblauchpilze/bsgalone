
target_include_directories (game_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (game_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/EntitiesUtils.cc
	)
