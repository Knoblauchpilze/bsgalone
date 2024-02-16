

#include "CircleBox.hh"

namespace bsgo {

CircleBox::CircleBox(const Eigen::Vector3f &center, const float radius) noexcept
  : m_center(center)
  , m_radius(radius)
  , m_squaredRadius(m_radius * m_radius)
{}

auto CircleBox::radius() const -> float
{
  return m_radius;
}

auto CircleBox::position() const -> Eigen::Vector3f
{
  return m_center;
}

void CircleBox::moveTo(const Eigen::Vector3f &position)
{
  m_center = position;
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
