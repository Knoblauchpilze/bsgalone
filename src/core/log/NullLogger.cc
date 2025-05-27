
#include "NullLogger.hh"

namespace core::log {

void NullLogger::setAllowLog(const bool /*allowLog*/) noexcept
{
  // Intentionally empty
}

void NullLogger::setLevel(const Severity /*severity*/) noexcept
{
  // Intentionally empty
}

void NullLogger::verbose(const std::string & /*message*/,
                         const std::string & /*module*/,
                         const std::string & /*service*/) const
{
  // Intentionally empty
}

void NullLogger::debug(const std::string & /*message*/,
                       const std::string & /*module*/,
                       const std::string & /*service*/) const
{
  // Intentionally empty
}

void NullLogger::info(const std::string & /*message*/,
                      const std::string & /*module*/,
                      const std::string & /*service*/) const
{
  // Intentionally empty
}

void NullLogger::notice(const std::string & /*message*/,
                        const std::string & /*module*/,
                        const std::string & /*service*/) const
{
  // Intentionally empty
}

void NullLogger::warn(const std::string & /*message*/,
                      const std::string & /*module*/,
                      const std::string & /*service*/,
                      const std::optional<std::string> & /*cause*/) const
{
  // Intentionally empty
}

void NullLogger::error(const std::string & /*message*/,
                       const std::string & /*module*/,
                       const std::string & /*service*/,
                       const std::optional<std::string> & /*cause*/) const
{
  // Intentionally empty
}

} // namespace core::log
