
target_include_directories (bsgalone_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/DockMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityAddedMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityDeletedMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EquipMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HangarMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpCancelledMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpRequestedMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PurchaseMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityMessageConsumer.cc
	)
