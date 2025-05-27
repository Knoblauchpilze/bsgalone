
#include "StdLogger.hh"
#include "StreamFormatter.hh"
#include <iomanip>
#include <iostream>
#include <sstream>

namespace core::log {

void StdLogger::setAllowLog(const bool allowLog) noexcept
{
  m_allowLog = allowLog;
}

void StdLogger::setLevel(const Severity severity) noexcept
{
  const std::lock_guard guard(m_locker);
  m_severity = severity;
}

void StdLogger::verbose(const std::string &message,
                        const std::string &module,
                        const std::string &service) const
{
  logTrace(Severity::VERBOSE, message, module, service, {});
}

void StdLogger::debug(const std::string &message,
                      const std::string &module,
                      const std::string &service) const
{
  logTrace(Severity::DEBUG, message, module, service, {});
}

void StdLogger::info(const std::string &message,
                     const std::string &module,
                     const std::string &service) const
{
  logTrace(Severity::INFO, message, module, service, {});
}

void StdLogger::notice(const std::string &message,
                       const std::string &module,
                       const std::string &service) const
{
  logTrace(Severity::NOTICE, message, module, service, {});
}

void StdLogger::warn(const std::string &message,
                     const std::string &module,
                     const std::string &service,
                     const std::optional<std::string> &cause) const
{
  logTrace(Severity::WARNING, message, module, service, cause);
}

void StdLogger::error(const std::string &message,
                      const std::string &module,
                      const std::string &service,
                      const std::optional<std::string> &cause) const
{
  logTrace(Severity::ERROR, message, module, service, cause);
}

namespace {
bool canBeDisplayed(const Severity severity, const Severity reference)
{
  return reference <= severity;
}

auto getTimestampAsStr() -> std::string
{
  auto currentTime      = std::time(nullptr);
  const auto *localTime = std::localtime(&currentTime);

  std::stringstream out;
  out << std::put_time(localTime, "%d-%m-%Y %H:%M:%S");

  return out.str();
}
} // namespace

void StdLogger::logTrace(const Severity severity,
                         const std::string &message,
                         const std::string &module,
                         const std::string &service,
                         const std::optional<std::string> &cause) const
{
  const std::lock_guard guard(m_locker);
  if (!m_allowLog || !canBeDisplayed(severity, m_severity))
  {
    return;
  }

  std::stringstream out;

  setStreamColor(out, Color::MAGENTA);
  out << service << " ";
  out << getTimestampAsStr() << " ";

  setStreamColorFromSeverity(out, severity);
  out << "[" << str(severity) << "]";
  out << " " << module << " ";
  clearStreamFormat(out);

  out << message;

  if (cause.has_value())
  {
    out << " (cause: \"" << *cause << "\")";
  }

  std::cout << out.str() << std::endl;
}

} // namespace core::log
