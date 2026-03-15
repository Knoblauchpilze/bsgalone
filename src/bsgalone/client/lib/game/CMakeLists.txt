
target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/events
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Game.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameMessageModule.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameSession.cc
	)
