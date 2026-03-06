
#include "LoadingScreenRenderer.hh"

namespace bsgalone::client {

constexpr auto LOADING_TEXTURE_FILE_PATH = "assets/loading_screen.png";

LoadingScreenRenderer::LoadingScreenRenderer()
  : bsgalone::client::DecalScreenRenderer(LOADING_TEXTURE_FILE_PATH)
{}

} // namespace bsgalone::client
