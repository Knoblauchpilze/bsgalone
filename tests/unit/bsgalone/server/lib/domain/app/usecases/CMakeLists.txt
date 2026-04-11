
target_include_directories (unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (unitTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/LoginUseCaseTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutUseCaseTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupUseCaseTest.cc
	)
