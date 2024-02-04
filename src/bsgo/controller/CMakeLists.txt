
target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Coordinator.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Systems.cc
	)

target_include_directories (bsgalone_core_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
