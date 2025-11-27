
target_include_directories (bsgo_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgo_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/SystemUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetUtils.cc
	)
