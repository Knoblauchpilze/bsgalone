
target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

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

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Coordinator.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DataSource.cc
	)
