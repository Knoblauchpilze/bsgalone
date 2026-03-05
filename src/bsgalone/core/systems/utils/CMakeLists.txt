
target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/SystemUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetUtils.cc
	)
