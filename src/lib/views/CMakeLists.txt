
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/IView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ServerView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShopView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemView.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
