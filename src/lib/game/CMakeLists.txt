
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Screen.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Game.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameMessageModule.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
