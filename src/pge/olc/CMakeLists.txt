
target_sources (pge_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/olcEngine.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VectorConversion.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ColorConversion.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PgeWrapper.cc
	)

target_include_directories (pge_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	)
