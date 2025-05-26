
/// @brief - A minimalistic bsg server implementation

#include "Server.hh"
#include <core_utils/CoreException.hh>
#include <core_utils/log/Locator.hh>
#include <core_utils/log/PrefixedLogger.hh>
#include <core_utils/log/StdLogger.hh>
#include <functional>

namespace {
// https://stackoverflow.com/questions/11468414/using-auto-and-lambda-to-handle-signal
std::function<void(int)> sigIntProcessing;
void sigIntInterceptor(const int signal)
{
  sigIntProcessing(signal);
}

// TODO: Could be moved to a common place
auto getPortFromEnvironmentVariable() -> int
{
  constexpr auto SERVER_PORT_ENV_VAR_NAME = "PORT";
  const auto maybeEnvVar                  = std::getenv(SERVER_PORT_ENV_VAR_NAME);

  if (maybeEnvVar == nullptr)
  {
    throw std::out_of_range("No port passed as environment variable to the server");
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
  utils::log::PrefixedLogger logger("server", "main");
  utils::log::Locator::provide(&raw);

  try
  {
    bsgo::Server server;
    sigIntProcessing = [&server](const int /*signal*/) { server.requestStop(); };
    // https://en.cppreference.com/w/cpp/utility/program/signal
    std::signal(SIGINT, sigIntInterceptor);

    server.run(getPortFromEnvironmentVariable());
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
