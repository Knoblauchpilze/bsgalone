
target_sources (client_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ResourceView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ServerView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipDbView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShopView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Views.cc
	)

target_include_directories (client_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
