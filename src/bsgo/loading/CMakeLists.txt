
target_include_directories (bsgo_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgo_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidDataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/BehaviorUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostDataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerDataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipDataSource.cc
	)
