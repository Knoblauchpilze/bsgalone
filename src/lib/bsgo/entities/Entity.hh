
#pragma once

#include "EntityKind.hh"
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

  auto str() const noexcept -> std::string;
};

} // namespace bsgo
