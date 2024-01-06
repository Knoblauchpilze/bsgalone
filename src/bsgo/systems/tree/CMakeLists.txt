
target_include_directories (bsgalone_core_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

target_sources (bsgalone_core_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/NodeState.cc
	${CMAKE_CURRENT_SOURCE_DIR}/INode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ControlNode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/FallbackNode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SequenceNode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RepeaterNode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/LeafNode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ConditionNode.cc
	)
