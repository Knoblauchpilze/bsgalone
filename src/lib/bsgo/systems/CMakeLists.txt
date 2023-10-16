
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ISystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MotionSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HealthSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PowerSystem.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotSystem.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
