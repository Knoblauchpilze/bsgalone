
#pragma once

#include <string>

namespace bsgalone::core {

enum class ContextKey
{
  PICKED_TARGET,
  TARGET_REACHED
};

auto str(const ContextKey &key) -> std::string;

} // namespace bsgalone::core
