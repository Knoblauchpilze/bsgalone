
#pragma once

#include "ILogger.hh"

namespace core::log {

class NullLogger : public ILogger
{
  public:
  void setAllowLog(const bool allowLog) noexcept override;

  void setLevel(const Severity severity) noexcept override;

  void verbose(const std::string &message,
               const std::string &module,
               const std::string &service) const override;

  void debug(const std::string &message,
             const std::string &module,
             const std::string &service) const override;

  void info(const std::string &message,
            const std::string &module,
            const std::string &service) const override;

  void notice(const std::string &message,
              const std::string &module,
              const std::string &service) const override;

  void warn(const std::string &message,
            const std::string &module,
            const std::string &service,
            const std::optional<std::string> &cause = {}) const override;

  void error(const std::string &message,
             const std::string &module,
             const std::string &service,
             const std::optional<std::string> &cause = {}) const override;
};

} // namespace core::log
