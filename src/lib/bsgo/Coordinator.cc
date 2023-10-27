
#include "Coordinator.hh"
#include "CircleBox.hh"
#include "ComputerSystem.hh"
#include "EffectSystem.hh"
#include "HealthSystem.hh"
#include "LootSystem.hh"
#include "MotionSystem.hh"
#include "PowerSystem.hh"
#include "TargetSystem.hh"
#include "WeaponEffectComponent.hh"
#include "WeaponSystem.hh"
#include <unordered_set>

namespace bsgo {
namespace {
template<typename ComponentType>
auto getComponent(const Uuid &ent,
                  const std::unordered_map<Uuid, std::shared_ptr<ComponentType>> &components)
  -> std::optional<std::shared_ptr<ComponentType>>
{
  const auto it = components.find(ent);
  if (components.end() == it)
  {
    return {};
  }

  return {it->second};
}

template<typename ComponentType>
auto getAllComponent(const Uuid &ent,
                     const std::unordered_multimap<Uuid, std::shared_ptr<ComponentType>> &components)
  -> std::vector<std::shared_ptr<ComponentType>>
{
  const auto range = components.equal_range(ent);
  if (range.first == range.second)
  {
    return {};
  }

  std::vector<std::shared_ptr<ComponentType>> out;
  for (auto it = range.first; it != range.second; ++it)
  {
    {
      out.push_back(it->second);
    }
  }

  return out;
}
} // namespace

Coordinator::Coordinator()
  : utils::CoreObject("coordinator")
{
  setService("bsgo");
  createSystems();
}

auto Coordinator::createEntity(const EntityKind &kind) -> Uuid
{
  auto ent = m_entities.size();
  m_entities.insert(ent);
  m_components.kinds[ent] = std::make_shared<KindComponent>(kind);
  return ent;
}

void Coordinator::addTransform(const Uuid &ent, IBoundingBoxPtr bbox)
{
  checkForOverrides(ent, "Transform", m_components.transforms);
  m_components.transforms[ent] = std::make_shared<TransformComponent>(std::move(bbox));
}

void Coordinator::addVelocity(const Uuid &ent, const float maxAcceleration)
{
  checkForOverrides(ent, "Velocity", m_components.velocities);
  m_components.velocities[ent] = std::make_shared<VelocityComponent>(maxAcceleration);
}

void Coordinator::addHealth(const Uuid &ent, const float hp, const float max, const float regen)
{
  checkForOverrides(ent, "Health", m_components.healths);
  m_components.healths[ent] = std::make_shared<HealthComponent>(hp, max, regen);
}

void Coordinator::addPower(const Uuid &ent, const float power, const float max, const float regen)
{
  checkForOverrides(ent, "Power", m_components.powers);
  m_components.powers[ent] = std::make_shared<PowerComponent>(power, max, regen);
}

void Coordinator::addTarget(const Uuid &ent)
{
  checkForOverrides(ent, "Target", m_components.targets);
  m_components.targets[ent] = std::make_shared<TargetComponent>();
}

void Coordinator::addFaction(const Uuid &ent, const Faction &faction)
{
  checkForOverrides(ent, "Faction", m_components.factions);
  m_components.factions[ent] = std::make_shared<FactionComponent>(faction);
}

void Coordinator::addLoot(const Uuid &ent, const float &amount)
{
  checkForOverrides(ent, "Loot", m_components.loots);
  m_components.loots[ent] = std::make_shared<LootComponent>(amount);
}

void Coordinator::addScanned(const Uuid &ent)
{
  checkForOverrides(ent, "Scanned", m_components.scanned);
  m_components.scanned[ent] = std::make_shared<ScannedComponent>();
}

void Coordinator::addPlayer(const Uuid &ent, const Uuid &player)
{
  checkForOverrides(ent, "Player", m_components.players);
  m_components.players[ent] = std::make_shared<PlayerComponent>(player);
}

void Coordinator::addLockerComponent(const Uuid &ent)
{
  checkForOverrides(ent, "Locker", m_components.lockers);
  m_components.lockers[ent] = std::make_shared<LockerComponent>();
}

void Coordinator::addWeapon(const Uuid &ent, const Weapon &weapon)
{
  checkEntityExist(ent, "Weapon");
  m_components.weapons.emplace(ent, std::make_shared<WeaponSlotComponent>(weapon));
}

void Coordinator::addComputer(const Uuid &ent, const Computer &computer)
{
  checkEntityExist(ent, "Computer");
  m_components.computers.emplace(ent, std::make_shared<ComputerSlotComponent>(computer));
}

void Coordinator::addWeaponEffect(const Uuid &ent,
                                  const utils::Duration &duration,
                                  const float damageModifier)
{
  checkEntityExist(ent, "WeaponEffect");
  m_components.effects.emplace(ent,
                               std::make_shared<WeaponEffectComponent>(duration, damageModifier));
}

void Coordinator::removeEffect(const Uuid &ent, const EffectComponentShPtr &effect)
{
  checkEntityExist(ent, "Effect");
  const auto range = m_components.effects.equal_range(ent);

  auto found = false;
  auto it    = range.first;
  while (!found && it != range.second)
  {
    if (it->second == effect)
    {
      m_components.effects.erase(it);
      found = true;
    }
    if (!found)
    {
      ++it;
    }
  }

  const auto entity = getEntity(ent);
  if (!found)
  {
    error("Failed to remove effect for entity " + entity.str());
  }

  log("Removed 1 effect for entity " + entity.str());
}

auto Coordinator::getEntity(const Uuid &ent) const -> Entity
{
  Entity out;
  out.uuid = ent;
  out.kind = *getComponent(ent, m_components.kinds);

  out.transform = getComponent(ent, m_components.transforms);
  out.velocity  = getComponent(ent, m_components.velocities);
  out.health    = getComponent(ent, m_components.healths);
  out.power     = getComponent(ent, m_components.powers);
  out.target    = getComponent(ent, m_components.targets);
  out.faction   = getComponent(ent, m_components.factions);
  out.loot      = getComponent(ent, m_components.loots);
  out.scanned   = getComponent(ent, m_components.scanned);
  out.player    = getComponent(ent, m_components.players);
  out.locker    = getComponent(ent, m_components.lockers);

  out.weapons   = getAllComponent(ent, m_components.weapons);
  out.computers = getAllComponent(ent, m_components.computers);
  out.effects   = getAllComponent(ent, m_components.effects);

  return out;
}

void Coordinator::deleteEntity(const Uuid &ent)
{
  if (!m_entities.contains(ent))
  {
    error("Failed to delete entity " + str(ent), "Unknown entity");
  }

  m_entities.erase(ent);

  m_components.transforms.erase(ent);
  m_components.velocities.erase(ent);
  m_components.healths.erase(ent);
  m_components.powers.erase(ent);
  m_components.targets.erase(ent);
  m_components.kinds.erase(ent);
  m_components.factions.erase(ent);
  m_components.loots.erase(ent);
  m_components.scanned.erase(ent);
  m_components.players.erase(ent);
  m_components.lockers.erase(ent);

  m_components.weapons.erase(ent);
  m_components.computers.erase(ent);
  m_components.effects.erase(ent);
}

auto Coordinator::getEntityAt(const Eigen::Vector3f &pos,
                              const std::optional<EntityKind> &filter) const -> std::optional<Uuid>
{
  std::optional<Uuid> out;
  float best = std::numeric_limits<float>::max();

  for (const auto &[uuid, transform] : m_components.transforms)
  {
    if (hasExpectedKind(uuid, filter) && transform->contains(pos))
    {
      const auto d = (pos - transform->position()).norm();
      if (d < best)
      {
        out  = uuid;
        best = d;
      }
    }
  }

  return out;
}

auto Coordinator::getEntitiesWithin(const IBoundingBox &bbox,
                                    const std::optional<EntityKind> &filter) const
  -> std::vector<Uuid>
{
  std::vector<Uuid> out;

  for (const auto &[uuid, transform] : m_components.transforms)
  {
    /// TODO: We should probably have a 'intersects' method.
    if (hasExpectedKind(uuid, filter) && bbox.isInside(transform->position()))
    {
      out.push_back(uuid);
    }
  }

  return out;
}

auto Coordinator::getEntitiesSatistying(const EntityPredicate &predicate) const
  -> std::vector<Entity>
{
  std::vector<Entity> out;

  for (const auto &id : m_entities)
  {
    const auto ent = getEntity(id);
    if (predicate(ent))
    {
      out.push_back(ent);
    }
  }

  return out;
}

void Coordinator::update(float elapsedSeconds)
{
  for (const auto &system : m_systems)
  {
    system->update(*this, elapsedSeconds);
  }

  cleanUpDeadEntities();
}

void Coordinator::createSystems()
{
  auto motion = std::make_unique<MotionSystem>();
  m_systems.push_back(std::move(motion));

  auto health = std::make_unique<HealthSystem>();
  m_systems.push_back(std::move(health));

  auto power = std::make_unique<PowerSystem>();
  m_systems.push_back(std::move(power));

  auto weapon = std::make_unique<WeaponSystem>();
  m_systems.push_back(std::move(weapon));

  auto target = std::make_unique<TargetSystem>();
  m_systems.push_back(std::move(target));

  auto computer = std::make_unique<ComputerSystem>();
  m_systems.push_back(std::move(computer));

  auto effect = std::make_unique<EffectSystem>();
  m_systems.push_back(std::move(effect));

  auto loot = std::make_unique<LootSystem>();
  m_systems.push_back(std::move(loot));
}

bool Coordinator::hasExpectedKind(const Uuid &ent, const std::optional<EntityKind> &kind) const
{
  if (!kind)
  {
    return true;
  }

  const auto entKind = m_components.kinds.at(ent);
  return entKind->kind() == *kind;
}

void Coordinator::checkEntityExist(const Uuid &ent, const std::string &componentName) const
{
  if (!m_entities.contains(ent))
  {
    error("Failed to add component " + componentName + " to unknown entity " + str(ent));
  }
}

template<typename ContainerType>
void Coordinator::checkForOverrides(const Uuid &ent,
                                    const std::string &componentName,
                                    const ContainerType &components) const
{
  checkEntityExist(ent, componentName);
  if (components.contains(ent))
  {
    warn("Overriding " + componentName + " for entity " + std::to_string(ent));
  }
}

void Coordinator::cleanUpDeadEntities()
{
  std::unordered_set<Uuid> deletedEntities;
  for (const auto &id : m_entities)
  {
    const auto ent = getEntity(id);
    if (ent.exists<HealthComponent>() && !ent.healthComp().isAlive())
    {
      deletedEntities.insert(id);
    }
  }

  for (const auto &ent : deletedEntities)
  {
    deleteEntity(ent);
  }

  if (!deletedEntities.empty())
  {
    log("Deleted " + std::to_string(deletedEntities.size()) + " entities");
  }
}

} // namespace bsgo
