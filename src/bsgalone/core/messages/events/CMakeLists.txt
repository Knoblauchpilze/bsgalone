
target_include_directories(bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/builders
	)

target_sources(bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/HangarDataMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LockerDataMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LogoutMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShopDataMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemDataMessage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/UndockMessage.cc
	)
