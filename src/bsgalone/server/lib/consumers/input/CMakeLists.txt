
target_include_directories (game_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (game_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingHelper.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JoinShipMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupMessageConsumer.cc
	)
