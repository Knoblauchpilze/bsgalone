
target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Entity.cc
	)

target_include_directories (bsgalone_core_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
