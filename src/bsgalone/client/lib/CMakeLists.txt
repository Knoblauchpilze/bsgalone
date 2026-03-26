
add_library (bsgalone_client_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/app
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/enums
	)

target_include_directories (bsgalone_client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (bsgalone_client_lib
	core_lib
	pge_lib
	)
