
#include "MapScreenRenderer.hh"

namespace bsgalone::client {

constexpr auto MAP_TEXTURE_FILE_PATH = "assets/map_bg.png";

MapScreenRenderer::MapScreenRenderer()
  : DecalScreenRenderer(MAP_TEXTURE_FILE_PATH)
{}

} // namespace bsgalone::client
