
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/olcEngine.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DecalWrapper.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TexturePack.cc
	${CMAKE_CURRENT_SOURCE_DIR}/FramerateControl.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RenderState.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PGEApp.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
