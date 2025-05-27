
#include "Severity.hh"

namespace core::log {

auto fromStr(const std::string &severity) noexcept -> Severity
{
  if (severity == "error")
  {
    return Severity::ERROR;
  }
  else if (severity == "warning")
  {
    return Severity::WARNING;
  }
  else if (severity == "notice")
  {
    return Severity::NOTICE;
  }
  else if (severity == "info")
  {
    return Severity::INFO;
  }
  else if (severity == "debug")
  {
    return Severity::DEBUG;
  }
  else
  {
    // Assume verbose.
    return Severity::VERBOSE;
  }
}

auto str(const Severity severity) -> std::string
{
  switch (severity)
  {
    case Severity::ERROR:
      return "error";
    case Severity::WARNING:
      return "warning";
    case Severity::NOTICE:
      return "notice";
    case Severity::INFO:
      return "info";
    case Severity::DEBUG:
      return "debug";
    case Severity::VERBOSE:
    default:
      return "verbose";
  }
}

} // namespace core::log
