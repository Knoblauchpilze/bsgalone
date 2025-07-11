
#include "LoginScreenRenderer.hh"

namespace pge {

constexpr auto LOGIN_TEXTURE_FILE_PATH = "assets/login_bg.png";

LoginScreenRenderer::LoginScreenRenderer()
  : DecalScreenRenderer("login", LOGIN_TEXTURE_FILE_PATH)
{}

} // namespace pge
