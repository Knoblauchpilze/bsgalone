
target_include_directories (bsgo_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (bsgo_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/NodeUtils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TargetNode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/FireNode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/FollowTargetNode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PickTargetNode.cc
	)
