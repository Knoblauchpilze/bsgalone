
target_sources (bsgo_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Tick.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TickDuration.cc
	)

target_include_directories (bsgo_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
