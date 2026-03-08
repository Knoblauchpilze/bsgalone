
target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/DockTransition.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameRole.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Item.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingTransition.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MessageType.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Uuid.cc
	)
