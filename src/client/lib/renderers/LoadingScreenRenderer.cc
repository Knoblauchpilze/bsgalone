
#include "LoadingScreenRenderer.hh"

namespace pge {

constexpr auto LOADING_TEXTURE_FILE_PATH = "assets/loading_screen.png";

LoadingScreenRenderer::LoadingScreenRenderer()
  : DecalScreenRenderer("loading", LOADING_TEXTURE_FILE_PATH)
{}

} // namespace pge
