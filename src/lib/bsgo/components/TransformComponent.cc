
#include "TransformComponent.hh"

namespace bsgo {

TransformComponent::TransformComponent(IBoundingBoxShPtr bbox)
  : m_bbox(std::move(bbox))
{}

auto TransformComponent::position() const -> Eigen::Vector3f
{
  return m_bbox->position();
}

bool TransformComponent::contains(const float x, const float y) const noexcept
{
  return !m_bbox || m_bbox->isInside(x, y);
}

} // namespace bsgo
