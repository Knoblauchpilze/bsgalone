
target_include_directories (bsgalone_server_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_server_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/EntityRemovedMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LootMessageConsumer.cc
	)

