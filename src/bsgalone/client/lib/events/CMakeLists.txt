
target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/queues
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/types
	)

target_sources (bsgalone_client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/LoginSucceededEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginFailedEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupSucceededEvent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupFailedEvent.cc
	)
