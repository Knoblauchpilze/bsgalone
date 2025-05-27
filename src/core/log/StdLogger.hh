
#pragma once

#include "ILogger.hh"
#include <mutex>

namespace core::log {

class StdLogger : public ILogger
{
  public:
  StdLogger()           = default;
  ~StdLogger() override = default;

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

  private:
  mutable std::mutex m_locker{};
  bool m_allowLog{true};
  Severity m_severity{Severity::DEBUG};

  void logTrace(const Severity severity,
                const std::string &message,
                const std::string &module,
                const std::string &service,
                const std::optional<std::string> &cause) const;
};

} // namespace core::log
