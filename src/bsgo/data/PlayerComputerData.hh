
#pragma once

#include "EntityKind.hh"
#include "TickDuration.hh"
#include "Uuid.hh"
#include <optional>
#include <unordered_set>

namespace bsgo {

struct PlayerComputerData
{
  Uuid dbId{};
  Uuid computerDbId{};

  std::string name{};
  int level{};
  bool offensive{};
  float powerCost{};
  std::optional<float> range{};
  TickDuration reloadTime{};

  std::optional<TickDuration> duration{};
  std::optional<std::unordered_set<EntityKind>> allowedTargets{};
  std::optional<float> damageModifier{};

  bool operator==(const PlayerComputerData &rhs) const;
};

} // namespace bsgo
