
#include "TransformSystem.hh"

namespace bsgo {

TransformSystem::TransformSystem()
  : utils::CoreObject("transform")
{
  setService("system");
}

void TransformSystem::addTransformComponent(const Uuid &ent, IBoundingBoxPtr bbox)
{
  if (m_transforms.contains(ent))
  {
    warn("Overriding transform for entity " + std::to_string(ent));
  }

  m_transforms[ent] = TransformComponent(std::move(bbox));
}

auto TransformSystem::getTransformComponent(const Uuid &ent) const
  -> std::optional<TransformComponent>
{
  const auto it = m_transforms.find(ent);
  if (m_transforms.end() == it)
  {
    return {};
  }

  return {it->second};
}

auto TransformSystem::getEntityAt(const Eigen::Vector3f &pos) const -> std::optional<Uuid>
{
  std::optional<Uuid> out;
  float best = std::numeric_limits<float>::max();

  for (const auto &[uuid, transform] : m_transforms)
  {
    if (transform.contains(pos))
    {
      const auto d = (pos - transform.position()).norm();
      if (d < best)
      {
        out  = uuid;
        best = d;
      }
    }
  }

  return out;
}

} // namespace bsgo
