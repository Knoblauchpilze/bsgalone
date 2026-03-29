
add_library (bsgalone_client_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/app
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/commands
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/enums
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/inputs
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/network
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/renderers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/ui
	)

target_link_libraries (bsgalone_client_lib PUBLIC
	bsgalone-interface-library
	core_lib
	pge_lib
	ui_lib
	messaging_lib
	bsgalone_core_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (bsgalone_client_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
