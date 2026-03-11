
#pragma once

#include "EntityKind.hh"
#include "TickDuration.hh"
#include "Uuid.hh"
#include <optional>
#include <unordered_set>

namespace bsgalone::core {

struct PlayerComputerData
{
  Uuid dbId{};
  Uuid computerDbId{};

  std::string name{};
  int level{};
  bool offensive{};
  float powerCost{};
  std::optional<float> range{};
  chrono::TickDuration reloadTime{};

  std::optional<chrono::TickDuration> duration{};
  std::optional<std::unordered_set<EntityKind>> allowedTargets{};
  std::optional<float> damageModifier{};

  bool operator==(const PlayerComputerData &rhs) const;
};

auto operator<<(std::ostream &out, const PlayerComputerData &data) -> std::ostream &;
bool operator>>(std::istream &in, PlayerComputerData &data);

} // namespace bsgalone::core
