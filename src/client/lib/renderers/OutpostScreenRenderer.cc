
#include "OutpostScreenRenderer.hh"

namespace pge {

constexpr auto OUTPOST_TEXTURE_FILE_PATH = "data/assets/outpost_bg.png";

OutpostScreenRenderer::OutpostScreenRenderer()
  : DecalScreenRenderer("outpost", OUTPOST_TEXTURE_FILE_PATH)
{}

} // namespace pge
