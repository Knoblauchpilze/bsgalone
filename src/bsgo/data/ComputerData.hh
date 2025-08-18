
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

  std::string name{};
  bool offensive{};
  float powerCost{};
  std::optional<float> range{};
  core::Duration reloadTime{};

  std::optional<core::Duration> duration{};
  std::optional<std::unordered_set<EntityKind>> allowedTargets{};
  std::optional<float> damageModifier{};

  bool operator==(const ComputerData &rhs) const;
};

} // namespace bsgo
