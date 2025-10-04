
#include "Coordinator.hh"
#include "CircleBox.hh"
#include "WeaponEffectComponent.hh"
#include <unordered_set>

#include "VectorUtils.hh"

namespace bsgo {
namespace {
template<typename ComponentType>
auto getComponent(const Uuid ent,
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
auto getAllComponent(const Uuid ent,
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

Coordinator::Coordinator(SystemsConfig &&config)
  : core::CoreObject("coordinator")
  , m_systems(std::make_unique<Systems>(std::move(config)))
{
  setService("bsgo");
}

void Coordinator::clear()
{
  m_nextEntity = Uuid{};
  m_entities.clear();
  m_components.clear();
}

auto Coordinator::createEntity(const EntityKind &kind) -> Uuid
{
  auto ent = m_nextEntity;
  ++m_nextEntity;
  m_entities.insert(ent);

  m_components.kinds[ent] = std::make_shared<KindComponent>(kind);
  return ent;
}

void Coordinator::addTransform(const Uuid ent, IBoundingBoxPtr bbox)
{
  checkForOverrides(ent, "Transform", m_components.transforms);
  m_components.transforms[ent] = std::make_shared<TransformComponent>(std::move(bbox));
}

void Coordinator::addVelocity(const Uuid ent, const VelocityData &data)
{
  checkForOverrides(ent, "Velocity", m_components.velocities);
  m_components.velocities[ent] = std::make_shared<VelocityComponent>(data);
}

void Coordinator::addHealth(const Uuid ent, const float hp, const float max, const float regen)
{
  checkForOverrides(ent, "Health", m_components.healths);
  m_components.healths[ent] = std::make_shared<HealthComponent>(hp, max, regen);
}

void Coordinator::addPower(const Uuid ent, const float power, const float max, const float regen)
{
  checkForOverrides(ent, "Power", m_components.powers);
  m_components.powers[ent] = std::make_shared<PowerComponent>(power, max, regen);
}

void Coordinator::addTarget(const Uuid ent)
{
  checkForOverrides(ent, "Target", m_components.targets);
  m_components.targets[ent] = std::make_shared<TargetComponent>();
}

void Coordinator::addTarget(const Uuid ent, const Uuid target)
{
  checkForOverrides(ent, "Target", m_components.targets);
  checkEntityExist(target, "Target");
  m_components.targets[ent] = std::make_shared<TargetComponent>(target);
}

void Coordinator::addFaction(const Uuid ent, const Faction &faction)
{
  checkForOverrides(ent, "Faction", m_components.factions);
  m_components.factions[ent] = std::make_shared<FactionComponent>(faction);
}

void Coordinator::addLoot(const Uuid ent)
{
  checkForOverrides(ent, "Loot", m_components.loots);
  m_components.loots[ent] = std::make_shared<LootComponent>();
}

void Coordinator::addScanned(const Uuid ent)
{
  checkForOverrides(ent, "Scanned", m_components.scanned);
  m_components.scanned[ent] = std::make_shared<ScannedComponent>();
}

void Coordinator::addOwner(const Uuid ent, const Uuid owner, const OwnerType &ownerType)
{
  checkForOverrides(ent, "Owner", m_components.owners);
  m_components.owners[ent] = std::make_shared<OwnerComponent>(owner, ownerType);
}

void Coordinator::addDamage(const Uuid ent, const float damage)
{
  checkForOverrides(ent, "Damage", m_components.damages);
  m_components.damages[ent] = std::make_shared<DamageComponent>(damage);
}

void Coordinator::addRemoval(const Uuid ent)
{
  checkForOverrides(ent, "Removal", m_components.removals);
  m_components.removals[ent] = std::make_shared<RemovalComponent>();
}

void Coordinator::addStatus(const Uuid ent,
                            const Status &status,
                            const std::optional<core::Duration> &jumpTime,
                            const std::optional<core::Duration> &threatJumpTime)
{
  checkForOverrides(ent, "Status", m_components.statuses);
  m_components.statuses[ent] = std::make_shared<StatusComponent>(status, jumpTime, threatJumpTime);
}

void Coordinator::addAI(const Uuid ent, INodePtr behavior)
{
  checkForOverrides(ent, "AI", m_components.ais);
  m_components.ais[ent] = std::make_shared<AIComponent>(std::move(behavior));
}

void Coordinator::addShipClass(const Uuid ent, const ShipClass &shipClass)
{
  checkForOverrides(ent, "ShipClass", m_components.shipClasses);
  m_components.shipClasses[ent] = std::make_shared<ShipClassComponent>(shipClass);
}

void Coordinator::addName(const Uuid ent, const std::string &name)
{
  checkForOverrides(ent, "Name", m_components.names);
  m_components.names[ent] = std::make_shared<NameComponent>(name);
}

void Coordinator::addNetworkSync(const Uuid ent, const std::unordered_set<ComponentType> &toSync)
{
  checkForOverrides(ent, "NetworkSync", m_components.networkSyncs);
  m_components.networkSyncs[ent] = std::make_shared<NetworkSyncComponent>(toSync);
}

void Coordinator::addDbId(const Uuid ent, const Uuid dbId)
{
  checkForOverrides(ent, "Db", m_components.dbs);
  m_components.dbs[ent] = std::make_shared<DbComponent>(dbId);
}

void Coordinator::addDbSync(const Uuid ent)
{
  checkForOverrides(ent, "DbSync", m_components.dbSyncs);
  m_components.dbSyncs[ent] = std::make_shared<DbSyncComponent>();
}

void Coordinator::addWeapon(const Uuid ent, const PlayerWeaponData &weapon)
{
  checkEntityExist(ent, "Weapon");
  m_components.weapons.emplace(ent, std::make_shared<WeaponSlotComponent>(weapon));
}

void Coordinator::addComputer(const Uuid ent, const PlayerComputerData &computer)
{
  checkEntityExist(ent, "Computer");
  m_components.computers.emplace(ent, std::make_shared<ComputerSlotComponent>(computer));
}

void Coordinator::addResourceComponent(const Uuid ent, const Uuid resource, const int amount)
{
  checkEntityExist(ent, "Resource");
  m_components.resources.emplace(ent, std::make_shared<ResourceComponent>(resource, amount));
}

void Coordinator::addWeaponEffect(const Uuid ent, const Tick &duration, const float damageModifier)
{
  checkEntityExist(ent, "WeaponEffect");
  m_components.effects.emplace(ent,
                               std::make_shared<WeaponEffectComponent>(duration, damageModifier));
}

void Coordinator::removeEffect(const Uuid ent, const EffectComponentShPtr &effect)
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

  debug("Removed 1 effect for entity " + entity.str());
}

auto Coordinator::getEntity(const Uuid ent) const -> Entity
{
  Entity out{};
  out.uuid       = ent;
  auto maybeKind = getComponent(ent, m_components.kinds);
  if (!maybeKind)
  {
    // The entity is probably dead, create a default component.
    maybeKind = std::make_shared<KindComponent>(EntityKind::NONE);
  }
  out.kind = *maybeKind;

  out.transform   = getComponent(ent, m_components.transforms);
  out.velocity    = getComponent(ent, m_components.velocities);
  out.health      = getComponent(ent, m_components.healths);
  out.power       = getComponent(ent, m_components.powers);
  out.target      = getComponent(ent, m_components.targets);
  out.faction     = getComponent(ent, m_components.factions);
  out.loot        = getComponent(ent, m_components.loots);
  out.scanned     = getComponent(ent, m_components.scanned);
  out.owner       = getComponent(ent, m_components.owners);
  out.damage      = getComponent(ent, m_components.damages);
  out.removal     = getComponent(ent, m_components.removals);
  out.status      = getComponent(ent, m_components.statuses);
  out.ai          = getComponent(ent, m_components.ais);
  out.shipClass   = getComponent(ent, m_components.shipClasses);
  out.name        = getComponent(ent, m_components.names);
  out.networkSync = getComponent(ent, m_components.networkSyncs);
  out.db          = getComponent(ent, m_components.dbs);
  out.dbSync      = getComponent(ent, m_components.dbSyncs);

  out.weapons   = getAllComponent(ent, m_components.weapons);
  out.computers = getAllComponent(ent, m_components.computers);
  out.effects   = getAllComponent(ent, m_components.effects);
  out.resources = getAllComponent(ent, m_components.resources);

  return out;
}

void Coordinator::deleteEntity(const Uuid ent)
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
  m_components.owners.erase(ent);
  m_components.damages.erase(ent);
  m_components.removals.erase(ent);
  m_components.statuses.erase(ent);
  m_components.ais.erase(ent);
  m_components.shipClasses.erase(ent);
  m_components.names.erase(ent);
  m_components.networkSyncs.erase(ent);
  m_components.dbs.erase(ent);
  m_components.dbSyncs.erase(ent);

  m_components.weapons.erase(ent);
  m_components.computers.erase(ent);
  m_components.effects.erase(ent);
  m_components.resources.erase(ent);
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

auto Coordinator::getEntitiesWithinSatistying(const IBoundingBox &bbox,
                                              const EntityPredicate &predicate) const
  -> std::vector<Entity>
{
  const auto entities = getEntitiesWithin(bbox);

  std::vector<Entity> out;

  for (const auto &id : entities)
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
  m_systems->update(*this, elapsedSeconds);
  cleanUpDeadEntities();
}

bool Coordinator::hasExpectedKind(const Uuid ent, const std::optional<EntityKind> &kind) const
{
  if (!kind)
  {
    return true;
  }

  const auto entKind = m_components.kinds.at(ent);
  return entKind->kind() == *kind;
}

void Coordinator::checkEntityExist(const Uuid ent, const std::string &componentName) const
{
  if (!m_entities.contains(ent))
  {
    error("Failed to add component " + componentName + " to unknown entity " + str(ent));
  }
}

template<typename ContainerType>
void Coordinator::checkForOverrides(const Uuid ent,
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
    if (ent.exists<RemovalComponent>() && ent.removalComp().toBeDeleted())
    {
      deletedEntities.insert(id);
    }
  }

  for (const auto &ent : deletedEntities)
  {
    deleteEntity(ent);
  }
}

} // namespace bsgo
