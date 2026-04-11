
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (unitTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/LoginRequestConsumerTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutRequestConsumerTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupRequestConsumerTest.cc
	)
