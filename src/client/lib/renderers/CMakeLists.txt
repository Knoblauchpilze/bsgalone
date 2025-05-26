
target_include_directories (client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/common
	)

target_sources (client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameScreenRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostScreenRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginScreenRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MapScreenRenderer.cc
	)
