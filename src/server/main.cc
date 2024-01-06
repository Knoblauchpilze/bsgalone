
/// @brief - A minimalistic bsg client implementation

#include "App.hh"
#include "AppDesc.hh"
#include "IsometricViewFrame.hh"
#include "TopViewFrame.hh"
#include <core_utils/CoreException.hh>
#include <core_utils/LoggerLocator.hh>
#include <core_utils/PrefixedLogger.hh>
#include <core_utils/StdLogger.hh>

int main(int /*argc*/, char ** /*argv*/)
{
  // Create the logger.
  utils::StdLogger raw;
  raw.setLevel(utils::Level::Debug);
  utils::PrefixedLogger logger("pge", "main");
  utils::LoggerLocator::provide(&raw);

  try
  {
    logger.logMessage(utils::Level::Notice, "Starting application");
  }
  catch (const utils::CoreException &e)
  {
    logger.logError(utils::Level::Critical,
                    "Caught internal exception while setting up application",
                    e.what());
    return EXIT_FAILURE;
  }
  catch (const std::exception &e)
  {
    logger.logError(utils::Level::Critical,
                    "Caught internal exception while setting up application",
                    e.what());
    return EXIT_FAILURE;
  }
  catch (...)
  {
    logger.logMessage(utils::Level::Critical, "Unexpected error while setting up application");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
