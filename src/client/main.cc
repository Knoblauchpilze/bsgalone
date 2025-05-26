
/// @brief - A minimalistic bsg client implementation

#include "App.hh"
#include "AppDesc.hh"
#include "TopViewFrame.hh"
#include <core_utils/CoreException.hh>
#include <core_utils/log/Locator.hh>
#include <core_utils/log/PrefixedLogger.hh>
#include <core_utils/log/StdLogger.hh>

namespace {
auto getPortFromEnvironmentVariable() -> int
{
  constexpr auto SERVER_PORT_ENV_VAR_NAME = "PORT";
  const auto maybeEnvVar                  = std::getenv(SERVER_PORT_ENV_VAR_NAME);

  if (maybeEnvVar == nullptr)
  {
    throw std::out_of_range("No port passed as environment variable to connect to the server");
  }

  int port = 0;
  try
  {
    port = std::stoi(maybeEnvVar);
  }
  catch (const std::invalid_argument &e)
  {
    std::string message("Failed to convert provided port: ");
    message += maybeEnvVar;
    throw std::invalid_argument(message);
  }
  catch (const std::out_of_range &e)
  {
    std::string message("Provided port is not valid: ");
    message += maybeEnvVar;
    throw std::invalid_argument(message);
  }

  if (port < 1 || port > 65535)
  {
    std::string message("Port should be in range ]1; 65536[, provided value is not: ");
    message += maybeEnvVar;
    throw std::out_of_range(message);
  }

  return port;
}
} // namespace

int main(int /*argc*/, char ** /*argv*/)
{
  // Create the logger.
  utils::log::StdLogger raw;
  raw.setLevel(utils::log::Severity::DEBUG);
  utils::log::PrefixedLogger logger("pge", "main");
  utils::log::Locator::provide(&raw);

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
    pge::App demo(ad, getPortFromEnvironmentVariable());

    demo.run();
  }
  catch (const utils::CoreException &e)
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
