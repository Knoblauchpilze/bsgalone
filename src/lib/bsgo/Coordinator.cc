
#include "Coordinator.hh"

namespace bsgo {

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

auto Coordinator::createEntity(const EntityKind &kind, IBoundingBoxPtr bbox) -> Uuid
{
  const auto ent = createEntity(kind);
  addTransform(ent, std::move(bbox));
  return ent;
}

auto Coordinator::createEntity(const EntityKind &kind,
                               IBoundingBoxPtr bbox,
                               const Eigen::Vector3f &speed) -> Uuid
{
  const auto ent = createEntity(kind);
  addTransform(ent, std::move(bbox));
  addVelocity(ent, speed);
  return ent;
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

  out.transform = getTransform(ent);
  out.velocity  = getVelocity(ent);

  return out;
}

auto Coordinator::getTransform(const Uuid &ent) const -> std::optional<TransformShPtr>
{
  const auto it = m_transforms.find(ent);
  if (m_transforms.end() == it)
  {
    return {};
  }

  return {it->second};
}

auto Coordinator::getVelocity(const Uuid &ent) const -> std::optional<VelocityShPtr>
{
  const auto it = m_velocities.find(ent);
  if (m_velocities.end() == it)
  {
    return {};
  }

  return {it->second};
}

auto Coordinator::getEntityAt(const Eigen::Vector3f &pos) const -> std::optional<Uuid>
{
  std::optional<Uuid> out;
  float best = std::numeric_limits<float>::max();

  for (const auto &[uuid, transform] : m_transforms)
  {
    if (transform->contains(pos))
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

auto Coordinator::getEntitiesWithin(const IBoundingBox &bbox) const -> std::vector<Uuid>
{
  std::vector<Uuid> out;

  for (const auto &[uuid, transform] : m_transforms)
  {
    /// TODO: We should probably have a 'intersects' method.
    if (bbox.isInside(transform->position()))
    {
      out.push_back(uuid);
    }
  }

  return out;
}

void Coordinator::addTransform(const Uuid &ent, IBoundingBoxPtr bbox)
{
  if (m_transforms.contains(ent))
  {
    warn("Overriding transform for entity " + std::to_string(ent));
  }

  m_transforms[ent] = std::make_shared<Transform>(std::move(bbox));
}

void Coordinator::addVelocity(const Uuid &ent, const Eigen::Vector3f &speed)
{
  if (m_velocities.contains(ent))
  {
    warn("Overriding velocity for entity " + std::to_string(ent));
  }

  m_velocities[ent] = std::make_shared<Velocity>(speed);
}

} // namespace bsgo
