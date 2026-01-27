
target_sources (client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ClientMessageQueue.cc
	)

target_include_directories (client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
