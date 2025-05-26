
target_include_directories (bsgo_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgo_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/JumpUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LockerUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PurchaseUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/StringUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VectorUtils.cc
	)
