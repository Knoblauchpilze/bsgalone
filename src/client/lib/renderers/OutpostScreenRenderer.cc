
#include "OutpostScreenRenderer.hh"

namespace pge {

constexpr auto OUTPOST_TEXTURE_FILE_PATH = "assets/outpost_bg.png";

OutpostScreenRenderer::OutpostScreenRenderer()
  : bsgalone::client::DecalScreenRenderer(OUTPOST_TEXTURE_FILE_PATH)
{}

} // namespace pge
