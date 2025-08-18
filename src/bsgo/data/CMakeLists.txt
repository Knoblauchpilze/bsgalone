
target_include_directories (bsgo_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgo_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidDataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComputerData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DatabaseEntityMapper.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostDataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerComputerData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerDataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerResourceData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerShipData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerWeaponData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ResourceData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipDataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponData.cc
	)
