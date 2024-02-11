
target_include_directories(bsgalone_tests PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources(bsgalone_tests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/MessageTypeTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SerializationBehaviorTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UuidTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/StringTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OptionalTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ConnectionMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DockMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityAddedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityRemovedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EquipMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HangarMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpCancelledMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpRequestedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LootMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PurchaseMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScannedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotComponentMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponComponentMessageTest.cc
	)
