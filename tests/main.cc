
# include <core_utils/StdLogger.hh>
# include <core_utils/PrefixedLogger.hh>
# include <core_utils/LoggerLocator.hh>
# include <core_utils/CoreException.hh>

# include <gtest/gtest.h>

int
main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  // Create the logger.
  utils::StdLogger raw;
  raw.setLevel(utils::Level::Debug);
  utils::PrefixedLogger logger("pge", "main");
  utils::LoggerLocator::provide(&raw);

  return RUN_ALL_TESTS();
}
