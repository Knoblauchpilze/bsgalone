
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AIComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComponentType.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ComputerSlotComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DamageComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DbComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EffectComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/FactionComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/HealthComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/KindComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LootComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/NameComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/NetworkComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OwnerComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PowerComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RegenerativeComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RemovalComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ResourceComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ScannedComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipClassComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/StatusComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TransformComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponEffectComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponSlotComponent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Components.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
