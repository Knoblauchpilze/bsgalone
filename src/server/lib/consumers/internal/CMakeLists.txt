
target_include_directories (server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AiBehaviorSyncMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityRemovedMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LootMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RoutingMessageConsumer.cc
	)
