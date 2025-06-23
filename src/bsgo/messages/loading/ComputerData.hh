
#pragma once

#include "EntityKind.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include <optional>
#include <unordered_set>

namespace bsgo {

struct ComputerData
{
  Uuid dbId{};
  Uuid computerDbId{};

  int level{};
  bool offensive{};
  float powerCost{};
  std::optional<float> range{};
  core::Duration reloadTime{};

  std::optional<core::Duration> duration{};
  std::optional<std::unordered_set<EntityKind>> allowedTargets{};
  std::optional<float> damageModifier{};

  bool operator==(const ComputerData &rhs) const;
};

auto serializeComputerData(std::ostream &out, const ComputerData &data) -> std::ostream &;
bool deserializeComputerData(std::istream &in, ComputerData &data);

} // namespace bsgo
