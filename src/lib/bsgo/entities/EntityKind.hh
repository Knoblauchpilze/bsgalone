
#pragma once

#include <string>

namespace bsgo {

enum class EntityKind
{
  NONE,
  SHIP,
  ASTEROID,
  OUTPOST,
  PLAYER
};

auto str(const EntityKind &kind) noexcept -> std::string;

} // namespace bsgo
