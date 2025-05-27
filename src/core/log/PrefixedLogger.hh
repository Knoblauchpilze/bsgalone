
#pragma once

#include "ILogger.hh"

namespace core::log {

class PrefixedLogger : public ILogger
{
  public:
  /// @brief - Create a wrapper around the specified logger with the specified module
  /// and service.
  /// @param service - the name of the service for this logger.
  /// @param module - the name of the module for this logger.
  PrefixedLogger(const std::string &service, const std::string &module);
  ~PrefixedLogger() override = default;

  /// @brief - Return the current module for this logger.
  /// @return - a string representing the module.
  auto getModule() const -> std::string;

  /// @brief - Return the current service for this logger.
  /// @return - a string representing the service.
  auto getService() const -> std::string;

  /// @brief - Defines a new module for this logger.
  /// @param module - the new module.
  void setModule(const std::string &module) noexcept;

  /// @brief - Add a new module to the list. It will be added at the end of the list
  /// and will thus show up at the end of the log messages.
  /// @param module - the new module to register.
  void addModule(const std::string &module) noexcept;

  /// @brief - Defines a new service for this logger.
  /// @param service - the new service.
  void setService(const std::string &service) noexcept;

  /// @brief - Add a new service to the list. It will be added at the end of the list
  /// and will thus show up at the end of the log messages.
  /// @param service - the new service to register.
  void addService(const std::string &service) noexcept;

  void setAllowLog(const bool allowLog) noexcept override;

  void setLevel(const Severity severity) noexcept override;

  void verbose(const std::string &message) const;
  void debug(const std::string &message) const;
  void info(const std::string &message) const;
  void notice(const std::string &message) const;
  void warn(const std::string &message, const std::optional<std::string> &cause = {}) const;
  void error(const std::string &message, const std::optional<std::string> &cause = {}) const;

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
  std::string m_service{};
  std::string m_module{};
};

} // namespace core::log
