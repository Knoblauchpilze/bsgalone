
#include "Coordinator.hh"

namespace bsgo {

Coordinator::Coordinator()
  : utils::CoreObject("coordinator")
{
  setService("bsgo");
}

auto Coordinator::createEntity() -> Uuid
{
  auto ent = m_entities.size();
  m_entities.insert(ent);
  return ent;
}

auto Coordinator::createEntityWithTransform(IBoundingBoxPtr bbox) -> Uuid
{
  const auto ent = createEntity();
  m_transformSystem.addTransformComponent(ent, std::move(bbox));
  return ent;
}

auto Coordinator::getEntity(const Uuid &ent) const -> Entity
{
  Entity out{.uuid = ent};

  out.transform = m_transformSystem.getTransformComponent(ent);

  return out;
}

auto Coordinator::getTransformSystem() -> TransformSystem &
{
  return m_transformSystem;
}

} // namespace bsgo
