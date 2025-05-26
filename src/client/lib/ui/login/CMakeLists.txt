
target_include_directories (client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/LoginScreenUiHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/CredentialsUiHandler.cc
	)
