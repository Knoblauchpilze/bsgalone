
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingFinishedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingStartedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerComputerDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerComputerListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerLoginDataMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerResourceDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerResourceListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerShipDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerShipListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerWeaponDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerWeaponListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ResourceDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ResourceListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemListMessageTest.cc
	)