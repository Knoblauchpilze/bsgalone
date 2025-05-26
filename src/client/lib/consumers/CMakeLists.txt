
target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ComponentMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/InternalMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ClientMessageConsumerUtils.cc
	)
