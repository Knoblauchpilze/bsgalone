
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

  /// @brief - Verify whether the component with the specified type exists in
  /// the entity.
  /// @return - true if the component exists.
  template<typename Component>
  bool exists() const;

  /// @brief - Safely attempts to access the component with the specified type and
  /// raises an exception if it either doesn't exist or is invalid.
  /// @tparam Component - the type of the component to access.
  /// @return - a reference to the component if it exists.
  template<typename Component>
  auto access() const -> const Component &;

  template<typename Component>
  auto access() -> Component &;
};

} // namespace bsgo
