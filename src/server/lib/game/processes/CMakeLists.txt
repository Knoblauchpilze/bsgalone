
target_include_directories (server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractProcess.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DbSyncProcess.cc
	${CMAKE_CURRENT_SOURCE_DIR}/IProcess.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Processes.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ProcessType.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RespawnProcess.cc
	)
