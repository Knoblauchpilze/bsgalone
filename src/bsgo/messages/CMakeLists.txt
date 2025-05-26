
target_include_directories (bsgo_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgo_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/MessageType.cc
	${CMAKE_CURRENT_SOURCE_DIR}/IMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ValidatableMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComponentUpdatedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComponentSyncMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ConnectionMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DockMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityRemovedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EntityAddedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EquipMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HangarMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpCancelledMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpRequestedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LootMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/NetworkMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PurchaseMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScannedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotComponentMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponComponentMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MessageParser.cc
	)

