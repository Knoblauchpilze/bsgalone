
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(unitTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/CreateAsteroidUseCaseTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HealthSystemTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/InitializeSystemUseCaseTest.cc
	)
