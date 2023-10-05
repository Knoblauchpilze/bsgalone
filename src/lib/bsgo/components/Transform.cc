
#include "Transform.hh"

namespace bsgo {

Transform::Transform(IBoundingBoxShPtr bbox)
  : m_bbox(std::move(bbox))
{}

auto Transform::position() const -> Eigen::Vector3f
{
  return m_bbox->position();
}

bool Transform::contains(const Eigen::Vector3f &pos) const noexcept
{
  return !m_bbox || m_bbox->isInside(pos);
}

void Transform::translate(const Eigen::Vector3f &delta)
{
  m_bbox->translate(delta);
}

} // namespace bsgo
