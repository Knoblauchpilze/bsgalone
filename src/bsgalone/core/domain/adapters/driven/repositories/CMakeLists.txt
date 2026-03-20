
target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AccountRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemRepository.cc
	)
