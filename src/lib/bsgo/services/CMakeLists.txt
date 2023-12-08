
target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Services.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SignupService.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PurchaseService.cc
	)
