
#include "Coordinator.hh"
#include "CircleBox.hh"

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
} // namespace

Coordinator::Coordinator()
  : utils::CoreObject("coordinator")
{
  setService("bsgo");
}

auto Coordinator::createEntity(const EntityKind &kind) -> Uuid
{
  auto ent        = m_entities.size();
  m_entities[ent] = kind;
  return ent;
}

void Coordinator::addTransform(const Uuid &ent, IBoundingBoxPtr bbox)
{
  if (!m_entities.contains(ent))
  {
    error("Unknown entity " + str(ent));
  }
  if (m_components.transforms.contains(ent))
  {
    warn("Overriding transform for entity " + std::to_string(ent));
  }

  m_components.transforms[ent] = std::make_shared<Transform>(std::move(bbox));
}

void Coordinator::addVelocity(const Uuid &ent, const Eigen::Vector3f &speed)
{
  if (!m_entities.contains(ent))
  {
    error("Unknown entity " + str(ent));
  }
  if (m_components.velocities.contains(ent))
  {
    warn("Overriding velocity for entity " + std::to_string(ent));
  }

  m_components.velocities[ent] = std::make_shared<Velocity>(Eigen::Vector3f::Zero(), speed);
}

void Coordinator::addHealth(const Uuid &ent, const float hp, const float max)
{
  if (!m_entities.contains(ent))
  {
    error("Unknown entity " + str(ent));
  }
  if (m_components.health.contains(ent))
  {
    warn("Overriding hull points for entity " + std::to_string(ent));
  }

  m_components.health[ent] = std::make_shared<Health>(hp, max);
}

void Coordinator::addPower(const Uuid &ent, const float power, const float max)
{
  if (!m_entities.contains(ent))
  {
    error("Unknown entity " + str(ent));
  }
  if (m_components.powers.contains(ent))
  {
    warn("Overriding power for entity " + std::to_string(ent));
  }

  m_components.powers[ent] = std::make_shared<Power>(power, max);
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
  out.health    = getComponent(ent, m_components.health);
  out.power     = getComponent(ent, m_components.powers);

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
  m_motionSystem.update(m_components, elapsedSeconds);
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

} // namespace bsgo
