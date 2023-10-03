
#pragma once

#include <string>

namespace bsgo {

enum class EntityKind
{
  SHIP,
  ASTEROID
};

auto str(const EntityKind &kind) noexcept -> std::string;

} // namespace bsgo
