
target_include_directories (bsgo_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgo_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessageListener.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessageQueue.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MessageProcessor.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsyncMessageQueue.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SynchronizedMessageQueue.cc
	)
