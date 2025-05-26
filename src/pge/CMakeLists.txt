
add_library (pge_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_compile_options(pge_lib PRIVATE
	-Wno-error=unknown-pragmas
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/olc
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/vector
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/colors
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/sprites
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/rendering
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/app
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/coordinates
	)

target_link_libraries (pge_lib PUBLIC
	bsgalone-interface-library
	png
	X11
	GL
	pthread
	)

if (${ENABLE_TESTS})
	target_link_libraries (pge_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
