

#include "CircleBox.hh"

namespace bsgo {

CircleBox::CircleBox(const Eigen::Vector3f &center, const float radius) noexcept
  : m_center(center)
  , m_radius(radius)
  , m_squaredRadius(m_radius * m_radius)
{}

auto CircleBox::position() const -> Eigen::Vector3f
{
  return m_center;
}

bool CircleBox::isInside(const float x, const float y) const noexcept
{
  Eigen::Vector3f p(x, y, 0.0f);
  return (p - m_center).squaredNorm() < m_squaredRadius;
}

} // namespace bsgo
