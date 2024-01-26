
target_include_directories (bsgalone_core_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessageListener.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessageQueue.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MessageProcessor.cc
	${CMAKE_CURRENT_SOURCE_DIR}/NetworkMessageQueue.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SynchronizedMessageQueue.cc
	)
