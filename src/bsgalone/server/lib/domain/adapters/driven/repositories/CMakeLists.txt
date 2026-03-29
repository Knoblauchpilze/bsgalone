
target_include_directories (bsgalone_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AccountRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DbConnection.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerRepository.cc
	)
