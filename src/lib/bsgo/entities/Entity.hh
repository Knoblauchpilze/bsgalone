
#pragma once

#include "EntityKind.hh"
#include "Health.hh"
#include "Power.hh"
#include "Transform.hh"
#include "Uuid.hh"
#include "Velocity.hh"
#include <optional>
#include <string>

namespace bsgo {

struct Entity
{
  Uuid uuid;
  EntityKind kind{EntityKind::NONE};
  std::optional<TransformShPtr> transform{};
  std::optional<VelocityShPtr> velocity{};
  std::optional<HealthShPtr> health{};
  std::optional<PowerShPtr> power{};

  auto str() const noexcept -> std::string;
};

} // namespace bsgo
