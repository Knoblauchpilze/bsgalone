
/// @brief - A minimalistic bsg server implementation

#include "Server.hh"
#include <core_utils/CoreException.hh>
#include <core_utils/log/Locator.hh>
#include <core_utils/log/PrefixedLogger.hh>
#include <core_utils/log/StdLogger.hh>
#include <functional>

#include "JumpMessage.hh"
#include "ScannedMessage.hh"
#include "VectorUtils.hh"
#include "VelocityMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace {
// https://stackoverflow.com/questions/11468414/using-auto-and-lambda-to-handle-signal
std::function<void(int)> sigIntProcessing;
void sigIntInterceptor(const int signal)
{
  sigIntProcessing(signal);
}
} // namespace

int main(int /*argc*/, char ** /*argv*/)
{
  // Create the logger.
  utils::log::StdLogger raw;
  raw.setLevel(utils::log::Severity::DEBUG);
  utils::log::PrefixedLogger logger("server", "main");
  utils::log::Locator::provide(&raw);

  try
  {
    bsgo::Server server;
    sigIntProcessing = [&server](const int /*signal*/) { server.requestStop(); };
    std::signal(SIGINT, sigIntInterceptor);

    constexpr auto DEFAULT_SERVER_PORT = 60000;
    server.run(DEFAULT_SERVER_PORT);
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
