
#include "CoreException.hh"
#include "log/Locator.hh"
#include "log/PrefixedLogger.hh"
#include "log/StdLogger.hh"

#include <gtest/gtest.h>

int main(int argc, char **argv)
{
  core::log::StdLogger raw;
  raw.setLevel(core::log::Severity::DEBUG);
  core::log::PrefixedLogger logger("unit", "main");
  core::log::Locator::provide(&raw);

  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
