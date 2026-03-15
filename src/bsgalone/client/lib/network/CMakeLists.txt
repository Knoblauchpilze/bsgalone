
target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/GameNetworkClient.cc
	${CMAKE_CURRENT_SOURCE_DIR}/InputGameMessageAdapter.cc
	${CMAKE_CURRENT_SOURCE_DIR}/NetworkEventListener.cc
	)
