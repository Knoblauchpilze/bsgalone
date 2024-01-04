
target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/DockMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JumpMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SlotMessageConsumer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/VelocityMessageConsumer.cc
	)
