
add_library (bsgalone_server_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (bsgalone_server_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/domain
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/events
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/clients
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/network
	)

target_sources (bsgalone_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Configurator.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Server.cc
	)

target_link_libraries (bsgalone_server_lib PUBLIC
	bsgalone-interface-library
	pq
	pqxx
	core_lib
	net_lib
	bsgalone_core_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (bsgalone_server_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
