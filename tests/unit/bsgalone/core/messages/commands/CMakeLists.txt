
target_include_directories (unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (unitTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/LoginRequestTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupRequestTest.cc
	)
