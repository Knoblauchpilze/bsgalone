
#pragma once

#include "EntityKind.hh"
#include "TransformComponent.hh"
#include "Uuid.hh"
#include <optional>
#include <string>

namespace bsgo {

struct Entity
{
  Uuid uuid;
  EntityKind kind;
  std::optional<TransformComponent> transform{};

  auto str() const noexcept -> std::string;
};

} // namespace bsgo
