
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
  m_transformSystem.addTransform(ent, std::move(bbox));
  return ent;
}

auto Coordinator::createEntity(const EntityKind &kind,
                               IBoundingBoxPtr bbox,
                               const Eigen::Vector3f &speed) -> Uuid
{
  const auto ent = createEntity(kind);
  m_transformSystem.addTransform(ent, std::move(bbox));
  m_velocitySystem.addVelocity(ent, speed);
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

  out.transform = m_transformSystem.getTransform(ent);
  out.velocity  = m_velocitySystem.getVelocity(ent);

  return out;
}

auto Coordinator::getTransformSystem() -> TransformSystem &
{
  return m_transformSystem;
}

auto Coordinator::getTransformSystem() const -> const TransformSystem &
{
  return m_transformSystem;
}

auto Coordinator::getVelocitySystem() -> VelocitySystem &
{
  return m_velocitySystem;
}

auto Coordinator::getVelocitySystem() const -> const VelocitySystem &
{
  return m_velocitySystem;
}

} // namespace bsgo
