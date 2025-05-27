
#pragma once

#include "Severity.hh"
#include <optional>
#include <string>

namespace core::log {

class ILogger
{
  public:
  virtual ~ILogger() = default;

  virtual void setAllowLog(const bool allowLog) noexcept = 0;

  virtual void setLevel(const Severity severity) noexcept = 0;

  virtual void verbose(const std::string &message,
                       const std::string &module,
                       const std::string &service) const = 0;

  virtual void debug(const std::string &message,
                     const std::string &module,
                     const std::string &service) const = 0;

  virtual void info(const std::string &message,
                    const std::string &module,
                    const std::string &service) const = 0;

  virtual void notice(const std::string &message,
                      const std::string &module,
                      const std::string &service) const = 0;

  virtual void warn(const std::string &message,
                    const std::string &module,
                    const std::string &service,
                    const std::optional<std::string> &cause = {}) const = 0;

  virtual void error(const std::string &message,
                     const std::string &module,
                     const std::string &service,
                     const std::optional<std::string> &cause = {}) const = 0;
};

} // namespace core::log
