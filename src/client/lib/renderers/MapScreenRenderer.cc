
#include "MapScreenRenderer.hh"

namespace pge {

constexpr auto MAP_TEXTURE_FILE_PATH = "data/assets/map_bg.png";

MapScreenRenderer::MapScreenRenderer()
  : DecalScreenRenderer("map", MAP_TEXTURE_FILE_PATH)
{}

} // namespace pge
