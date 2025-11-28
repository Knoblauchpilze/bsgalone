
#pragma once

#include <string>

namespace bsgo {

enum class ContextKey
{
  TARGET_REACHED
};

auto str(const ContextKey &key) -> std::string;

} // namespace bsgo
