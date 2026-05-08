
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/clients
	)

target_sources(unitTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/GameEventPublisherTest.cc
	)
