
#pragma once

#include "CoreException.hh"
#include "log/PrefixedLogger.hh"
#include <functional>
#include <string>

namespace core {

class CoreObject
{
  public:
  CoreObject(const std::string &name);
  CoreObject(const std::string &name, const bool allowLog);
  virtual ~CoreObject() = default;

  auto getName() const -> const std::string &;
  void setAllowLog(const bool allowLog) noexcept;

  protected:
  /// @brief - Define the unique service for the object. It resets any existing
  /// service.
  /// @param service - the unique service to assign to this object.
  void setService(const std::string &service);

  /// @brief - Add a new module to the list. It will be added at the end of the
  /// list and will thus show up at the end of the log messages.
  /// @param module - the new module to register.
  void addModule(const std::string &module);

  void verbose(const std::string &message) const;
  void debug(const std::string &message) const;
  void info(const std::string &message) const;
  void notice(const std::string &message) const;
  void warn(const std::string &message, const std::optional<std::string> &cause = {}) const;
  void error(const std::string &message, const std::optional<std::string> &cause = {}) const;
  void error(const std::string &message, const CoreException &cause) const;

  void withSafetyNet(std::function<void(void)> func, const std::string &functionName) const;

  private:
  std::string m_name{};
  log::PrefixedLogger m_logger;
};

} // namespace core
