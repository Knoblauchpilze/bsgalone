
#include "Environment.hh"
#include "GameServer.hh"
#include "SafetyNet.hh"
#include "log/Locator.hh"
#include "log/PrefixedLogger.hh"
#include "log/StdLogger.hh"
#include <csignal>

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
  core::log::StdLogger raw;
  raw.setLevel(core::log::Severity::DEBUG);
  core::log::PrefixedLogger logger("server", "main");
  core::log::Locator::provide(&raw);

  bsgalone::server::GameServer server;
  const auto port = core::getPortFromEnvironmentVariable();

  sigIntProcessing = [&server](const int /*signal*/) { server.requestStop(); };
  // https://en.cppreference.com/w/cpp/utility/program/signal
  std::signal(SIGINT, sigIntInterceptor);

  auto gameFunc = [&server, port]() { server.run(port); };

  if (!core::launchProtected(gameFunc, "main", logger))
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
