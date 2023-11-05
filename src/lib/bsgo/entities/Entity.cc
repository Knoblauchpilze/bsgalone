
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
                                + bsgo::str(entity.kind->kind()) + " to have a " + compName
                                + " component");
  }
  if (nullptr == *comp)
  {
    throw std::invalid_argument("Expected entity " + bsgo::str(entity.uuid) + "/"
                                + bsgo::str(entity.kind->kind()) + " to have a valid " + compName
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
  out += bsgo::str(kind->kind());

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
bool Entity::exists<FactionComponent>() const
{
  return details::checkComponentExists(faction);
}

template<>
bool Entity::exists<LootComponent>() const
{
  return details::checkComponentExists(loot);
}

template<>
bool Entity::exists<ScannedComponent>() const
{
  return details::checkComponentExists(scanned);
}

template<>
bool Entity::exists<OwnerComponent>() const
{
  return details::checkComponentExists(owner);
}

template<>
bool Entity::exists<DamageComponent>() const
{
  return details::checkComponentExists(damage);
}

template<>
bool Entity::exists<RemovalComponent>() const
{
  return details::checkComponentExists(removal);
}

template<>
bool Entity::exists<StatusComponent>() const
{
  return details::checkComponentExists(status);
}

template<>
bool Entity::exists<AIComponent>() const
{
  return details::checkComponentExists(ai);
}

auto Entity::transformComp() const -> const TransformComponent &
{
  return details::safeConstAccess(transform, *this, "Transform");
}

auto Entity::velocityComp() const -> const VelocityComponent &
{
  return details::safeConstAccess(velocity, *this, "Velocity");
}

auto Entity::healthComp() const -> const HealthComponent &
{
  return details::safeConstAccess(health, *this, "Health");
}

auto Entity::powerComp() const -> const PowerComponent &
{
  return details::safeConstAccess(power, *this, "Power");
}

auto Entity::targetComp() const -> const TargetComponent &
{
  return details::safeConstAccess(target, *this, "Target");
}

auto Entity::factionComp() const -> const FactionComponent &
{
  return details::safeConstAccess(faction, *this, "Faction");
}

auto Entity::lootComp() const -> const LootComponent &
{
  return details::safeConstAccess(loot, *this, "Loot");
}

auto Entity::scannedComp() const -> const ScannedComponent &
{
  return details::safeConstAccess(scanned, *this, "Scanned");
}

auto Entity::ownerComp() const -> const OwnerComponent &
{
  return details::safeConstAccess(owner, *this, "Owner");
}

auto Entity::damageComp() const -> const DamageComponent &
{
  return details::safeConstAccess(damage, *this, "Damage");
}

auto Entity::removalComp() const -> const RemovalComponent &
{
  return details::safeConstAccess(removal, *this, "Removal");
}

auto Entity::statusComp() const -> const StatusComponent &
{
  return details::safeConstAccess(status, *this, "Status");
}

auto Entity::aiComp() const -> const AIComponent &
{
  return details::safeConstAccess(ai, *this, "AI");
}

auto Entity::transformComp() -> TransformComponent &
{
  return details::safeAccess(transform, *this, "Transform");
}

auto Entity::velocityComp() -> VelocityComponent &
{
  return details::safeAccess(velocity, *this, "Velocity");
}

auto Entity::healthComp() -> HealthComponent &
{
  return details::safeAccess(health, *this, "Health");
}

auto Entity::powerComp() -> PowerComponent &
{
  return details::safeAccess(power, *this, "Power");
}

auto Entity::targetComp() -> TargetComponent &
{
  return details::safeAccess(target, *this, "Target");
}

auto Entity::lootComp() -> LootComponent &
{
  return details::safeAccess(loot, *this, "Loot");
}

auto Entity::scannedComp() -> ScannedComponent &
{
  return details::safeAccess(scanned, *this, "Scanned");
}

auto Entity::removalComp() -> RemovalComponent &
{
  return details::safeAccess(removal, *this, "Removal");
}

auto Entity::statusComp() -> StatusComponent &
{
  return details::safeAccess(status, *this, "Status");
}

auto Entity::aiComp() -> AIComponent &
{
  return details::safeAccess(ai, *this, "AI");
}

} // namespace bsgo
