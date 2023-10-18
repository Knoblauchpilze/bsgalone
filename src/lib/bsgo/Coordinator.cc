
#include "Coordinator.hh"
#include "CircleBox.hh"
#include "HealthSystem.hh"
#include "MotionSystem.hh"
#include "PowerSystem.hh"

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
  auto ent        = m_entities.size();
  m_entities[ent] = kind;
  return ent;
}

void Coordinator::addTransform(const Uuid &ent, IBoundingBoxPtr bbox)
{
  checkForOverrides(ent, "transform", m_components.transforms);
  m_components.transforms[ent] = std::make_shared<TransformComponent>(std::move(bbox));
}

void Coordinator::addVelocity(const Uuid &ent, const float maxAcceleration)
{
  checkForOverrides(ent, "velocity", m_components.velocities);
  m_components.velocities[ent] = std::make_shared<VelocityComponent>(maxAcceleration);
}

void Coordinator::addHealth(const Uuid &ent, const float hp, const float max, const float regen)
{
  checkForOverrides(ent, "health", m_components.healths);
  m_components.healths[ent] = std::make_shared<HealthComponent>(hp, max, regen);
}

void Coordinator::addPower(const Uuid &ent, const float power, const float max, const float regen)
{
  checkForOverrides(ent, "power", m_components.powers);
  m_components.powers[ent] = std::make_shared<PowerComponent>(power, max, regen);
}

void Coordinator::addWeapon(const Uuid &ent, const Weapon &weapon)
{
  checkEntityExist(ent, "weapon");
  m_components.weapons.emplace(ent, std::make_shared<WeaponSlotComponent>(weapon));
}

void Coordinator::addTarget(const Uuid &ent)
{
  checkForOverrides(ent, "target", m_components.targets);
  m_components.targets[ent] = std::make_shared<TargetComponent>();
}

auto Coordinator::getEntity(const Uuid &ent) const -> Entity
{
  Entity out;
  out.uuid = ent;

  const auto it = m_entities.find(ent);
  if (it != m_entities.end())
  {
    out.kind = it->second;
  }

  out.transform = getComponent(ent, m_components.transforms);
  out.velocity  = getComponent(ent, m_components.velocities);
  out.health    = getComponent(ent, m_components.healths);
  out.power     = getComponent(ent, m_components.powers);

  out.weapons = getAllComponent(ent, m_components.weapons);

  out.target = getComponent(ent, m_components.targets);

  return out;
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

void Coordinator::update(float elapsedSeconds)
{
  for (const auto &system : m_systems)
  {
    system->update(m_components, elapsedSeconds);
  }
}

void Coordinator::createSystems()
{
  auto motion = std::make_unique<MotionSystem>();
  m_systems.push_back(std::move(motion));

  auto health = std::make_unique<HealthSystem>();
  m_systems.push_back(std::move(health));

  auto power = std::make_unique<PowerSystem>();
  m_systems.push_back(std::move(power));
}

bool Coordinator::hasExpectedKind(const Uuid &ent, const std::optional<EntityKind> &kind) const
{
  if (!kind)
  {
    return true;
  }

  const auto it = m_entities.find(ent);
  return it != m_entities.end() && it->second == *kind;
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

} // namespace bsgo
