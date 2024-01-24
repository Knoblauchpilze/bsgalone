
target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ClientConnection.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ClientMessageQueue.cc
	)

target_include_directories (bsgalone_client_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
