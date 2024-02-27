
target_sources (pge_app_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Transparency.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Color.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ColorNames.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ColorSpace.cc
	)

target_include_directories (pge_app_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
