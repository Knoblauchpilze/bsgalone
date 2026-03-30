
target_include_directories (bsgalone_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ClientEventListener.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutputGameEventAdapter.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ServerEventListener.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ServerNetworkClient.cc
	)
