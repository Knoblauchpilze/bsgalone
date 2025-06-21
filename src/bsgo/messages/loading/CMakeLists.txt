
target_include_directories (bsgo_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgo_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidListMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingFinishedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingStartedMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostListMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerListMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipListMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponData.cc
	)

