
target_include_directories (net_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (net_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Connection.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Context.cc
	)
