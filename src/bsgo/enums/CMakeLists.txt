
target_sources (bsgo_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Faction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameRole.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingTransition.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipClass.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Status.cc
	)

target_include_directories (bsgo_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
