
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AsteroidRepository.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemRepository.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
