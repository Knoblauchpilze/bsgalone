
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerShipRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComputerRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidLootRepository.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
