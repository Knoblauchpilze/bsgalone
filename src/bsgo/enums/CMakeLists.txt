
target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/EntityKind.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Faction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Slot.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Item.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Status.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipClass.cc
	)

target_include_directories (bsgalone_core_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
