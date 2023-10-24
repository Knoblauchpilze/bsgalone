
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/SystemUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ISystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MotionSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HealthSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PowerSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComputerSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EffectSystem.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
