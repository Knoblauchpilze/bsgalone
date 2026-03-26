
#include "App.hh"
#include "AppDesc.hh"
#include "Environment.hh"
#include "SafetyNet.hh"
#include "TopViewFrame.hh"
#include "log/Locator.hh"
#include "log/PrefixedLogger.hh"
#include "log/StdLogger.hh"

constexpr auto USERNAME_ENV_KEY  = "USERNAME";
constexpr auto PASSWORD_ENV_KEY  = "PASSWORD";
constexpr auto GAME_ROLE_ENV_KEY = "GAMEROLE";

int main(int /*argc*/, char ** /*argv*/)
{
  core::log::StdLogger raw;
  raw.setLevel(core::log::Severity::DEBUG);
  core::log::PrefixedLogger logger("client", "main");
  core::log::Locator::provide(&raw);

  auto tiles  = pge::CenteredViewport({0.0f, 0.0f}, {16.0f, 12.0f});
  auto pixels = pge::TopLeftViewport({0.0f, 0.0f}, {800.0f, 600.0f});

  pge::AppDesc desc{.dims       = pge::Vec2i{800, 600},
                    .frame      = std::make_shared<pge::TopViewFrame>(tiles, pixels),
                    .name       = "bsgalone",
                    .fixedFrame = true,
                    .maxFps     = 20};

  bsgalone::client::App app(desc);

  auto gameFunc = [&app]() { app.run(); };

  if (!core::launchProtected(gameFunc, "main", logger))
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
