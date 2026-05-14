
target_include_directories(bsgalone_core_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/bbox
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/components
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/entities
	)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/enums
	)

target_sources(bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Uuid.cc
	)
