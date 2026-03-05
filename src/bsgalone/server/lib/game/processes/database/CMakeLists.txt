
target_include_directories (bsgalone_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidSynchronizer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DatabaseSynchronizer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerShipSynchronizer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemOutpostSynchronizer.cc
	)
