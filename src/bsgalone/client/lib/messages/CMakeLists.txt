
target_include_directories(bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/HangarDataMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemDataMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UndockMessageConsumer.cc
	)
