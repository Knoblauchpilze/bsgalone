
#include "Entity.hh"

namespace bsgo {
namespace details {
template<typename Component>
inline bool checkComponentExists(const std::optional<std::shared_ptr<Component>> &comp)
{
  return comp && (nullptr != *comp);
}

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

auto Entity::str() const noexcept -> std::string
{
  std::string out("[");

  out += std::to_string(uuid);
  out += ",";
  out += bsgo::str(kind);

  if (transform)
  {
    const auto p = (*transform)->position();
    out += ",";
    out += std::to_string(p(0));
    out += "x";
    out += std::to_string(p(1));
    out += "x";
    out += std::to_string(p(2));
  }

  out += "]";
  return out;
}

template<>
bool Entity::exists<TransformComponent>() const
{
  return details::checkComponentExists(transform);
}

template<>
bool Entity::exists<VelocityComponent>() const
{
  return details::checkComponentExists(velocity);
}

template<>
bool Entity::exists<HealthComponent>() const
{
  return details::checkComponentExists(health);
}

template<>
bool Entity::exists<PowerComponent>() const
{
  return details::checkComponentExists(power);
}

template<>
bool Entity::exists<TargetComponent>() const
{
  return details::checkComponentExists(target);
}

template<>
auto Entity::access<TransformComponent>() const -> const TransformComponent &
{
  return details::safeConstAccess(transform, *this, "Transform");
}

template<>
auto Entity::access<VelocityComponent>() const -> const VelocityComponent &
{
  return details::safeConstAccess(velocity, *this, "Velocity");
}

template<>
auto Entity::access<HealthComponent>() const -> const HealthComponent &
{
  return details::safeConstAccess(health, *this, "Health");
}

template<>
auto Entity::access<PowerComponent>() const -> const PowerComponent &
{
  return details::safeConstAccess(power, *this, "Power");
}

template<>
auto Entity::access<TargetComponent>() const -> const TargetComponent &
{
  return details::safeConstAccess(target, *this, "Target");
}

template<>
auto Entity::access<TransformComponent>() -> TransformComponent &
{
  return details::safeAccess(transform, *this, "Transform");
}

template<>
auto Entity::access<VelocityComponent>() -> VelocityComponent &
{
  return details::safeAccess(velocity, *this, "Velocity");
}

template<>
auto Entity::access<HealthComponent>() -> HealthComponent &
{
  return details::safeAccess(health, *this, "Heath");
}

template<>
auto Entity::access<PowerComponent>() -> PowerComponent &
{
  return details::safeAccess(power, *this, "Power");
}

template<>
auto Entity::access<TargetComponent>() -> TargetComponent &
{
  return details::safeAccess(target, *this, "Target");
}

} // namespace bsgo
