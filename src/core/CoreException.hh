
#pragma once

#include <optional>
#include <stdexcept>
#include <string>

namespace core {

class CoreException : public std::exception
{
  public:
  CoreException(const std::string &message,
                const std::string &module,
                const std::string &service,
                const std::optional<std::string> &cause) noexcept;

  CoreException(const std::string &message,
                const std::string &module,
                const std::string &service,
                const CoreException &cause) noexcept;

  ~CoreException() noexcept override = default;

  auto what() const throw() -> const char * override;

  private:
  std::string m_message{};
};

} // namespace core
