
target_include_directories (game_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (game_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/DockTransition.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Item.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MessageType.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Slot.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Uuid.cc
	)
