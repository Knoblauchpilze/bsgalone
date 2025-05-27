
#include "PrefixedLogger.hh"
#include "Locator.hh"

namespace core::log {
namespace {
auto consolidate(const std::string &str) -> std::string
{
  if (str.empty() || str[0] == '[')
  {
    return str;
  }

  return "[" + str + "]";
}
} // namespace

PrefixedLogger::PrefixedLogger(const std::string &service, const std::string &module)
  : ILogger()
  , m_service(consolidate(service))
  , m_module(consolidate(module))
{}

auto PrefixedLogger::getModule() const -> std::string
{
  return m_module;
}

auto PrefixedLogger::getService() const -> std::string
{
  return m_service;
}

void PrefixedLogger::setModule(const std::string &module) noexcept
{
  if (!module.empty())
  {
    m_module = consolidate(module);
  }
}

void PrefixedLogger::addModule(const std::string &module) noexcept
{
  if (!module.empty())
  {
    m_module += " " + consolidate(module);
  }
}

void PrefixedLogger::setService(const std::string &service) noexcept
{
  if (!service.empty())
  {
    m_service = consolidate(service);
  }
}

void PrefixedLogger::addService(const std::string &service) noexcept
{
  if (!service.empty())
  {
    m_service += " " + consolidate(service);
  }
}

void PrefixedLogger::setAllowLog(const bool allowLog) noexcept
{
  Locator::getLogger().setAllowLog(allowLog);
}

void PrefixedLogger::setLevel(const Severity severity) noexcept
{
  Locator::getLogger().setLevel(severity);
}

void PrefixedLogger::verbose(const std::string &message) const
{
  Locator::getLogger().verbose(message, m_module, m_service);
}
void PrefixedLogger::debug(const std::string &message) const
{
  Locator::getLogger().debug(message, m_module, m_service);
}
void PrefixedLogger::info(const std::string &message) const
{
  Locator::getLogger().info(message, m_module, m_service);
}
void PrefixedLogger::notice(const std::string &message) const
{
  Locator::getLogger().notice(message, m_module, m_service);
}
void PrefixedLogger::warn(const std::string &message, const std::optional<std::string> &cause) const
{
  Locator::getLogger().warn(message, m_module, m_service, cause);
}
void PrefixedLogger::error(const std::string &message, const std::optional<std::string> &cause) const
{
  Locator::getLogger().error(message, m_module, m_service, cause);
}

namespace {
auto concatenate(const std::string &str, const std::string &suffix)
{
  auto out = consolidate(str);
  if (!suffix.empty())
  {
    out += " " + consolidate(suffix);
  }

  return out;
}
} // namespace

void PrefixedLogger::verbose(const std::string &message,
                             const std::string &module,
                             const std::string &service) const
{
  Locator::getLogger().verbose(message,
                               concatenate(m_module, module),
                               concatenate(m_service, service));
}

void PrefixedLogger::debug(const std::string &message,
                           const std::string &module,
                           const std::string &service) const
{
  Locator::getLogger().debug(message,
                             concatenate(m_module, module),
                             concatenate(m_service, service));
}

void PrefixedLogger::info(const std::string &message,
                          const std::string &module,
                          const std::string &service) const
{
  Locator::getLogger().info(message, concatenate(m_module, module), concatenate(m_service, service));
}

void PrefixedLogger::notice(const std::string &message,
                            const std::string &module,
                            const std::string &service) const
{
  Locator::getLogger().notice(message,
                              concatenate(m_module, module),
                              concatenate(m_service, service));
}

void PrefixedLogger::warn(const std::string &message,
                          const std::string &module,
                          const std::string &service,
                          const std::optional<std::string> &cause) const
{
  Locator::getLogger().warn(message,
                            concatenate(m_module, module),
                            concatenate(m_service, service),
                            cause);
}

void PrefixedLogger::error(const std::string &message,
                           const std::string &module,
                           const std::string &service,
                           const std::optional<std::string> &cause) const
{
  Locator::getLogger().error(message,
                             concatenate(m_module, module),
                             concatenate(m_service, service),
                             cause);
}

} // namespace core::log
