
target_include_directories (bsgalone_core_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/DataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidDataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostDataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerDataSource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipDataSource.cc
	)
