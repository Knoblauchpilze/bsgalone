
add_library (pge_app_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_compile_options(pge_app_lib PRIVATE
	-Wno-error=unknown-pragmas
	-Wno-error=unused-function)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/app
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/coordinates
	)

target_link_libraries(pge_app_lib
	png
	X11
	GL
	pthread
	core_utils
	)
