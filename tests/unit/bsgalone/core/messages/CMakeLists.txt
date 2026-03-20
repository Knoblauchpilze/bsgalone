
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/commands
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/credentials
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/loading
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/player
	)

target_sources(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/Comparison.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AiBehaviorSyncMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComponentSyncMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityAddedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityRemovedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HangarMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JoinShipMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LootMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScannedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotComponentMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponComponentMessageTest.cc
	)
