
#pragma once

#include "EntityKind.hh"
#include "Tick.hh"
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
  Tick reloadTime{};

  std::optional<Tick> duration{};
  std::optional<std::unordered_set<EntityKind>> allowedTargets{};
  std::optional<float> damageModifier{};

  bool operator==(const PlayerComputerData &rhs) const;
};

} // namespace bsgo
