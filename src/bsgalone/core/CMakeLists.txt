
add_library (bsgalone_core_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/bbox
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/common
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/components
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/consumers
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/controller
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/data
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/domain
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/entities
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/enums
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/listeners
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/loading
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/messages
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/network
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/queues
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/repositories
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/systems
	)

target_link_libraries (bsgalone_core_lib PUBLIC
	bsgalone-interface-library
	pq
	pqxx
	net_lib
	messaging_lib
	time_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (bsgalone_core_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
