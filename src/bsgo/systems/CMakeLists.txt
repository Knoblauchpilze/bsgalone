
target_include_directories (bsgalone_core_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/tree
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/behavior
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/SystemUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemType.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ISystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AISystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/BulletSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComputerSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EffectSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HealthSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LootSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MotionSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/NetworkSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PowerSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RemovalSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/StatusSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponSystem.cc
	)
