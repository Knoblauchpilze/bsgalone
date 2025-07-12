
#include "Environment.hh"

namespace core {

constexpr auto SERVER_PORT_ENV_VAR_NAME = "PORT";

auto getPortFromEnvironmentVariable() -> int
{
  const auto port = getEnvironmentVariable<int>(SERVER_PORT_ENV_VAR_NAME);

  if (port < 1 || port > 65535)
  {
    throw CoreException("Invalid value provided for port, it should be in range ]1; 65536[",
                        "core",
                        "environment",
                        "Provided value: " + std::to_string(port) + " is not in range");
  }

  return port;
}

} // namespace core