
#include "SafetyNet.hh"
#include "log/Locator.hh"
#include "log/PrefixedLogger.hh"
#include "log/StdLogger.hh"

int main(int /*argc*/, char ** /*argv*/)
{
  core::log::StdLogger raw;
  raw.setLevel(core::log::Severity::DEBUG);
  core::log::PrefixedLogger logger("client", "main");
  core::log::Locator::provide(&raw);

  auto gameFunc = []() {};

  if (!core::launchProtected(gameFunc, "main", logger))
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
