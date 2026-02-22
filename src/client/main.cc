
/// @brief - A minimalistic bsg client implementation

#include "App.hh"
#include "AppDesc.hh"
#include "CoreException.hh"
#include "Environment.hh"
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

  try
  {
    logger.notice("Starting application");

    auto tiles  = pge::CenteredViewport({0.0f, 0.0f}, {16.0f, 12.0f});
    auto pixels = pge::TopLeftViewport({0.0f, 0.0f}, {800.0f, 600.0f});

    pge::AppDesc ad{.dims       = pge::Vec2i{800, 600},
                    .frame      = std::make_shared<pge::TopViewFrame>(tiles, pixels),
                    .name       = "bsgalone",
                    .fixedFrame = true,
                    .maxFps     = 20};

    const auto port          = core::getPortFromEnvironmentVariable();
    const auto maybeUserName = core::tryGetEnvironmentVariable<std::string>(USERNAME_ENV_KEY);
    const auto maybePassword = core::tryGetEnvironmentVariable<std::string>(PASSWORD_ENV_KEY);
    const auto maybeGameRoleString = core::tryGetEnvironmentVariable<std::string>(GAME_ROLE_ENV_KEY);
    std::optional<bsgo::GameRole> maybeGameRole;
    if (maybeGameRoleString)
    {
      maybeGameRole = bsgo::fromDbGameRole(*maybeGameRoleString);
    }

    pge::App demo(ad, port, maybeUserName, maybePassword, maybeGameRole);

    demo.run();
  }
  catch (const core::CoreException &e)
  {
    logger.error("Caught internal exception while setting up application", e.what());
    return EXIT_FAILURE;
  }
  catch (const std::exception &e)
  {
    logger.error("Caught internal exception while setting up application", e.what());
    return EXIT_FAILURE;
  }
  catch (...)
  {
    logger.error("Unexpected error while setting up application");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
