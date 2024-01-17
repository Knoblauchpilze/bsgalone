
target_include_directories(bsgalone_tests PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources(bsgalone_tests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/MessageTypeTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UuidTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/StringTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OptionalTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DockMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EquipMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HangarMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LootMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PurchaseMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScannedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityMessageTest.cc
	)
