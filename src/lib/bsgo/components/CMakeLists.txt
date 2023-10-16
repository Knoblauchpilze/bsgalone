
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Transform.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Velocity.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RegenerativeComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Health.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Power.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponSlot.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
