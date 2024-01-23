
target_include_directories (bsgalone_core_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ConsumerUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DockMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EquipMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HangarMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpCancelledMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpRequestedMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PurchaseMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetMessageConsumer.cc
	)
