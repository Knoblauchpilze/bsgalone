
target_include_directories (server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingHelper.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JoinShipMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupMessageConsumer.cc
	)
