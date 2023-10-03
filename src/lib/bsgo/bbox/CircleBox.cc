

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

bool CircleBox::isInside(const Eigen::Vector3f &pos) const noexcept
{
  return (pos - m_center).squaredNorm() < m_squaredRadius;
}

void CircleBox::translate(const Eigen::Vector3f &delta)
{
  m_center += delta;
}

} // namespace bsgo
