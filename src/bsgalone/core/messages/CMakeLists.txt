
target_include_directories (game_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/player
	)

target_sources (game_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractPlayerMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractSystemMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/IMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/NetworkMessage.cc
	)
