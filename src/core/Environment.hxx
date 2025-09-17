
#pragma once

#include "CoreException.hh"
#include "Environment.hh"
#include <cstdlib>

namespace core {

template<>
auto tryGetEnvironmentVariable(const std::string &key) -> std::optional<std::string>
{
  const auto maybeEnvVar = std::getenv(key.c_str());
  if (maybeEnvVar == nullptr)
  {
    return {};
  }

  return maybeEnvVar;
}

template<>
auto tryGetEnvironmentVariable(const std::string &key) -> std::optional<int>
{
  const auto maybeVarAsString = tryGetEnvironmentVariable<std::string>(key);
  if (!maybeVarAsString)
  {
    return {};
  }

  int value = 0;
  try
  {
    value = std::stoi(*maybeVarAsString);
  }
  catch (const std::invalid_argument &e)
  {
    // Voluntarily not rethrowing but returning an empty optional.
    return {};
  }
  catch (const std::out_of_range &e)
  {
    // Voluntarily not rethrowing but returning an empty optional.
    return {};
  }

  return value;
}

template<typename T>
auto getEnvironmentVariable(const std::string &key, const T &defaultValue) -> T
{
  const auto maybeEnvVar = tryGetEnvironmentVariable<T>(key);
  if (!maybeEnvVar)
  {
    return defaultValue;
  }

  return *maybeEnvVar;
}

template<typename T>
auto getEnvironmentVariable(const std::string &key) -> T
{
  const auto maybeEnvVar = tryGetEnvironmentVariable<T>(key);
  if (!maybeEnvVar)
  {
    throw CoreException("Failed to get environment variable with key " + key,
                        "core",
                        "environment",
                        "Environment variable is not set");
  }

  return *maybeEnvVar;
}

} // namespace core
