
#pragma once

#include <string>

namespace bsgo {

enum class ContextKey
{
  PICKED_TARGET,
  TARGET_REACHED
};

auto str(const ContextKey &key) -> std::string;

} // namespace bsgo
