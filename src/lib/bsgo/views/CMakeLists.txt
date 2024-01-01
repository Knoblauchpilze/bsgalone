
target_sources (bsgalone_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AbstractView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ResourceView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShipView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ShopView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SystemView.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ServerView.cc
	)

target_include_directories (bsgalone_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
