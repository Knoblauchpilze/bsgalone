
#pragma once

#include "TransformComponent.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgo {

struct Entity
{
  Uuid uuid;
  std::optional<TransformComponent> transform{};
};

} // namespace bsgo
