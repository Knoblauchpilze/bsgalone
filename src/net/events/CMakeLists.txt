
target_include_directories (net_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (net_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ConnectionEstablishedEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ConnectionLostEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ClientConnectedEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ClientDisconnectedEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DataReadFailureEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DataReceivedEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DataSentEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DataWriteFailureEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ServerStartedEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ServerStoppedEvent.cc
	)
