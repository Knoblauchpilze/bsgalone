
#include "OutpostScreenRenderer.hh"

namespace bsgalone::client {

constexpr auto OUTPOST_TEXTURE_FILE_PATH = "assets/outpost_bg.png";

OutpostScreenRenderer::OutpostScreenRenderer()
  : DecalScreenRenderer(OUTPOST_TEXTURE_FILE_PATH)
{}

} // namespace bsgalone::client
