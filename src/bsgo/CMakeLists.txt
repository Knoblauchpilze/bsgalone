
add_library (bsgo_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

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
	${CMAKE_CURRENT_SOURCE_DIR}/entities
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/enums
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/messages
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

target_include_directories (bsgo_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_link_libraries (bsgo_lib PUBLIC
	bsgalone-interface-library
	pq
	pqxx
	core_lib
	net_lib
	)

if (${ENABLE_TESTS})
	target_link_libraries (bsgo_lib PUBLIC
		bsgalone-interface-test-library
		)
endif ()
