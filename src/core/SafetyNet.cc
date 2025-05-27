
#include "SafetyNet.hh"
#include "CoreException.hh"
#include "log/Locator.hh"

namespace core {

bool launchProtected(std::function<void(void)> func,
                     const std::string &functionName,
                     const std::string &module,
                     const std::string &service)
{
  try
  {
    func();

    return true;
  }
  catch (const CoreException &e)
  {
    log::Locator::getLogger().error("Caught exception while executing \"" + functionName + "\"",
                                    module,
                                    service,
                                    e.what());
  }
  catch (const std::exception &e)
  {
    log::Locator::getLogger().error("Caught unexpected exception while executing \"" + functionName
                                      + "\"",
                                    module,
                                    service,
                                    e.what());
  }
  catch (...)
  {
    log::Locator::getLogger().error("Unknown error while executing \"" + functionName + "\"",
                                    module,
                                    service);
  }

  return false;
}

bool launchProtected(std::function<void(void)> func,
                     const std::string &functionName,
                     const log::PrefixedLogger &logger)
{
  try
  {
    func();

    return true;
  }
  catch (const CoreException &e)
  {
    logger.error("Caught exception while executing \"" + functionName + "\"", e.what());
  }
  catch (const std::exception &e)
  {
    logger.error("Caught unexpected exception while executing \"" + functionName + "\"", e.what());
  }
  catch (...)
  {
    logger.error("Unknown error while executing \"" + functionName + "\"");
  }

  return false;
}

} // namespace core
