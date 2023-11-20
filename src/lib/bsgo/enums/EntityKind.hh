
#pragma once

#include <string>

namespace bsgo {

enum class EntityKind
{
  NONE,
  SHIP,
  ASTEROID,
  OUTPOST,
  PLAYER,
  BULLET
};

auto str(const EntityKind &kind) noexcept -> std::string;
auto toDbEntityKind(const EntityKind &kind) -> std::string;
auto fromDbEntityKind(const std::string &dbKind) -> EntityKind;

} // namespace bsgo
