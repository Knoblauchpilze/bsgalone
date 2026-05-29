
target_include_directories(tests_helper_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(tests_helper_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/TestGameEventPublisher.cc
	)
