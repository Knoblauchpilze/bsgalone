
target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/loading
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/player
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractPlayerCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractPlayerMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractSystemMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AiBehaviorSyncMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComponentUpdatedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComponentSyncMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityRemovedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityAddedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HangarMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JoinShipMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LootMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MessageParser.cc
	${CMAKE_CURRENT_SOURCE_DIR}/NetworkMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScannedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotComponentMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponComponentMessage.cc
	)
