
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/DbConnection.cc
	${CMAKE_CURRENT_SOURCE_DIR}/IRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidLootRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComputerPriceRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComputerRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerComputerRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerResourceRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerShipRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerWeaponRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ResourceRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipComputerRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipWeaponRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponPriceRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Repositories.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
