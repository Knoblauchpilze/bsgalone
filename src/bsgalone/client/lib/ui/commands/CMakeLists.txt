
target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ExitCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpRequestedCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemSelectedCommand.cc
	)
