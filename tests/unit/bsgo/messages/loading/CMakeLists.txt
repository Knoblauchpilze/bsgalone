
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComputerDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingFinishedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoadingStartedMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerLoginDataMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipDataTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipListMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponDataTest.cc
	)