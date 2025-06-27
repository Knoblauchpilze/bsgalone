
#include "CoreException.hh"
#include "log/Locator.hh"
#include <execinfo.h>
#include <vector>

namespace core {
namespace {
constexpr auto MAX_STACK_TRACE_DEPTH = 32u;

auto retrieveStackTrace() -> std::string
{
  std::vector<void *> addresses(MAX_STACK_TRACE_DEPTH);
  const auto size = backtrace(addresses.data(), addresses.size());
  addresses.resize(size);

  const auto funcs = backtrace_symbols(addresses.data(), addresses.size());

  std::string stackTrace{};
  for (auto i = 0u; i < addresses.size(); ++i)
  {
    stackTrace += " at ";
    stackTrace += funcs[i];
    stackTrace += "\n";
  }

  return stackTrace;
}
} // namespace

CoreException::CoreException(const std::string &message,
                             const std::string &module,
                             const std::string &service,
                             const std::optional<std::string> &cause) noexcept
  : std::exception()
  , m_message(message)
{
  log::Locator::getLogger().error(message, module, service, cause);
  log::Locator::getLogger().error(retrieveStackTrace(), module, service);
}

CoreException::CoreException(const std::string &message,
                             const std::string &module,
                             const std::string &service,
                             const CoreException &cause) noexcept
  : std::exception()
  , m_message(message)
{
  log::Locator::getLogger().error(message, module, service, cause.what());
  log::Locator::getLogger().error(retrieveStackTrace(), module, service);
}

const char *CoreException::what() const throw()
{
  return m_message.c_str();
}

} // namespace core
