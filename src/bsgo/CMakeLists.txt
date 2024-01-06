
add_library (bsgalone_core_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package (libpqxx REQUIRED)


add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/enums
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/common
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/bbox
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/components
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/entities
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/systems
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/repositories
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/views
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/services
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/messages
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/communication
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/data
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Coordinator.cc
	)

target_link_libraries (bsgalone_core_lib
	pq
	pqxx
	)

target_include_directories (bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
