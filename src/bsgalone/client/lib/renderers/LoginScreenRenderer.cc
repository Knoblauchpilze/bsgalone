
#include "LoginScreenRenderer.hh"

namespace bsgalone::client {

constexpr auto LOGIN_TEXTURE_FILE_PATH = "assets/login_bg.png";

LoginScreenRenderer::LoginScreenRenderer()
  : DecalScreenRenderer(LOGIN_TEXTURE_FILE_PATH)
{}

} // namespace bsgalone::client
