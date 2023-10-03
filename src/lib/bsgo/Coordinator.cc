
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

auto Coordinator::createEntityWithTransform(const EntityKind &kind, IBoundingBoxPtr bbox) -> Uuid
{
  const auto ent = createEntity(kind);
  m_transformSystem.addTransformComponent(ent, std::move(bbox));
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

  out.transform = m_transformSystem.getTransformComponent(ent);

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

} // namespace bsgo
