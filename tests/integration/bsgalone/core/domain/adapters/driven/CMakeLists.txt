
target_include_directories(integrationTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(integrationTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidRepositoryTest.cc
	)
