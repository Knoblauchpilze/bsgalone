
target_include_directories (client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/GameColorUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MessageUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MessageListenerWrapper.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScreenCommon.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipItemUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotComponentUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TimeStepUtils.cc
	)
