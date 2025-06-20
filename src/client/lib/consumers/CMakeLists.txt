
target_include_directories (client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ClientMessageConsumerUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComponentMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/InternalMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerListMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemMessageConsumer.cc
	)
