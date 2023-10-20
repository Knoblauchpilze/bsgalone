
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TransformComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RegenerativeComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HealthComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PowerComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponSlotComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/FactionComponent.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
