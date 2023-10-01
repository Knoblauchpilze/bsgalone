
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

void Coordinator::addTransformComponent(const Uuid &ent, IBoundingBoxPtr bbox)
{
  if (!m_entities.contains(ent))
  {
    error("Invalid entity", std::to_string(ent) + " is not registered");
  }

  if (m_transforms.contains(ent))
  {
    warn("Overriding transform for entity " + std::to_string(ent));
  }

  m_transforms[ent] = TransformComponent(std::move(bbox));
}

auto Coordinator::getTransformComponent(const Uuid &ent) const -> std::optional<TransformComponent>
{
  if (!m_entities.contains(ent))
  {
    error("Invalid entity", std::to_string(ent) + " is not registered");
  }

  const auto it = m_transforms.find(ent);
  if (m_transforms.end() == it)
  {
    return {};
  }

  return {it->second};
}

} // namespace bsgo
