
#include "CoreObject.hh"
#include "SafetyNet.hh"

namespace core {

CoreObject::CoreObject(const std::string &name)
  : CoreObject(name, true)
{}

CoreObject::CoreObject(const std::string &name, const bool allowLog)
  : m_name(name)
  , m_logger("", name)
{
  setAllowLog(allowLog);
}

auto CoreObject::getName() const -> const std::string &
{
  return m_name;
}

void CoreObject::setAllowLog(const bool allowLog) noexcept
{
  m_logger.setAllowLog(allowLog);
}

void CoreObject::setService(const std::string &service)
{
  m_logger.setService(service);
}

void CoreObject::addModule(const std::string &module)
{
  m_logger.addModule(module);
}

void CoreObject::verbose(const std::string &message) const
{
  m_logger.verbose(message);
}

void CoreObject::debug(const std::string &message) const
{
  m_logger.debug(message);
}

void CoreObject::info(const std::string &message) const
{
  m_logger.info(message);
}

void CoreObject::notice(const std::string &message) const
{
  m_logger.notice(message);
}

void CoreObject::warn(const std::string &message, const std::optional<std::string> &cause) const
{
  m_logger.warn(message, cause);
}

void CoreObject::error(const std::string &message, const std::optional<std::string> &cause) const
{
  throw CoreException(message, m_logger.getModule(), m_logger.getService(), cause);
}

void CoreObject::error(const std::string &message, const CoreException &cause) const
{
  throw CoreException(message, m_logger.getModule(), m_logger.getService(), cause);
}

bool CoreObject::withSafetyNet(std::function<void(void)> func, const std::string &functionName) const
{
  return launchProtected(func, functionName, m_logger);
}

} // namespace core
