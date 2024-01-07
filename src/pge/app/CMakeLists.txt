
target_sources (pge_app_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Controls.cc
	${CMAKE_CURRENT_SOURCE_DIR}/FramerateControl.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RenderState.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PGEApp.cc
	)

target_include_directories (pge_app_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
