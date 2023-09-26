
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/MapView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OutpostView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameView.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
