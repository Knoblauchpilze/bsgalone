
#include "Velocity.hh"

namespace bsgo {

Velocity::Velocity(const Eigen::Vector3f &speed)
  : m_speed(speed)
{}

auto Velocity::speed() const noexcept -> Eigen::Vector3f
{
  return m_speed;
}

void Velocity::accelerate(const Eigen::Vector3f &acc)
{
  m_speed = acc;
}

} // namespace bsgo
