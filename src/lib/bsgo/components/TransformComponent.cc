
#include "TransformComponent.hh"

namespace bsgo {

TransformComponent::TransformComponent(IBoundingBoxShPtr bbox)
  : m_bbox(std::move(bbox))
{}

auto TransformComponent::position() const -> Eigen::Vector3f
{
  return m_bbox->position();
}

bool TransformComponent::contains(const Eigen::Vector3f &pos) const noexcept
{
  return !m_bbox || m_bbox->isInside(pos);
}

void TransformComponent::translate(const Eigen::Vector3f &delta)
{
  m_bbox->translate(delta);
}

} // namespace bsgo
