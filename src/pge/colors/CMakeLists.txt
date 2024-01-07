
target_sources (pge_app_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Transparency.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ColorSpace.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Colors.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Color.cc
	)

target_include_directories (pge_app_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
