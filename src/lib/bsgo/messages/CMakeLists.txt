
target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/MessageType.cc
	${CMAKE_CURRENT_SOURCE_DIR}/IMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DockMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HangarMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LootMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScannedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityMessage.cc
	)

