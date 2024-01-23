
target_include_directories (bsgalone_core_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/network
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessageListener.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessageQueue.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MessageQueue.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SynchronizedMessageQueue.cc
	)
