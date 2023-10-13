
#pragma once

#include "Entity.hh"

namespace bsgo {
namespace details {
template<typename Component>
inline void checkAccessIsSafe(const std::optional<std::shared_ptr<Component>> &comp,
                              const Entity &entity,
                              const std::string &compName)
{
  if (!comp)
  {
    throw std::invalid_argument("Expected entity " + bsgo::str(entity.uuid) + "/"
                                + bsgo::str(entity.kind) + " to have a " + compName + " component");
  }
  if (nullptr == *comp)
  {
    throw std::invalid_argument("Expected entity " + bsgo::str(entity.uuid) + "/"
                                + bsgo::str(entity.kind) + " to have a valid " + compName
                                + " component");
  }
}

template<typename Component>
inline auto safeConstAccess(const std::optional<std::shared_ptr<Component>> &comp,
                            const Entity &entity,
                            const std::string &compName) -> const Component &
{
  checkAccessIsSafe(comp, entity, compName);
  return **comp;
}

template<typename Component>
inline auto safeAccess(const std::optional<std::shared_ptr<Component>> &comp,
                       const Entity &entity,
                       const std::string &compName) -> Component &
{
  checkAccessIsSafe(comp, entity, compName);
  return **comp;
}
} // namespace details

template<>
inline auto Entity::access<Transform>() const -> const Transform &
{
  return details::safeConstAccess<Transform>(transform, *this, "Transform");
}

template<>
inline auto Entity::access<Velocity>() const -> const Velocity &
{
  return details::safeConstAccess(velocity, *this, "Velocity");
}

template<>
inline auto Entity::access<Health>() const -> const Health &
{
  return details::safeConstAccess(health, *this, "Health");
}

template<>
inline auto Entity::access<Power>() const -> const Power &
{
  return details::safeConstAccess(power, *this, "Power");
}

template<>
inline auto Entity::access<Velocity>() -> Velocity &
{
  return details::safeAccess(velocity, *this, "Velocity");
}

} // namespace bsgo
