
target_include_directories(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources(unitTests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/HangarDataMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LockerDataMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShopDataMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemDataMessageTest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UndockMessageTest.cc
	)
