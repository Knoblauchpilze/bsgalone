
#pragma once

#include "TransformComponent.hh"
#include "Uuid.hh"
#include <optional>
#include <string>

namespace bsgo {

struct Entity
{
  Uuid uuid;
  std::optional<TransformComponent> transform{};

  auto str() const noexcept -> std::string;
};

} // namespace bsgo
