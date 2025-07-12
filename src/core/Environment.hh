
#pragma once

#include <optional>
#include <string>

namespace core {

template<typename T>
auto tryGetEnvironmentVariable(const std::string &key) -> std::optional<T>;

template<typename T>
auto getEnvironmentVariable(const std::string &key) -> T;

auto getPortFromEnvironmentVariable() -> int;

} // namespace core

#include "Environment.hxx"
