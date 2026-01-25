
#include "AppDesc.hh"
#include "Environment.hh"
#include "GameApp.hh"
#include "SafetyNet.hh"
#include "TopViewFrame.hh"
#include "log/Locator.hh"
#include "log/PrefixedLogger.hh"
#include "log/StdLogger.hh"

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

  const auto port = core::getPortFromEnvironmentVariable();
  bsgalone::client::GameApp app(desc, port);

  auto gameFunc = [&app]() { app.run(); };

  if (!core::launchProtected(gameFunc, "main", logger))
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
