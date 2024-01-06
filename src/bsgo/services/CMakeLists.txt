
target_include_directories (bsgalone_core_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LoginService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LockerService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PurchaseService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Services.cc
	)
