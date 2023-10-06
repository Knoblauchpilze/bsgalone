
#include "Velocity.hh"

namespace bsgo {

Velocity::Velocity(const Eigen::Vector3f &acceleration, const Eigen::Vector3f &speed)
  : m_acceleration(acceleration)
  , m_speed(speed)
{}

auto Velocity::acceleration() const noexcept -> Eigen::Vector3f
{
  return m_acceleration;
}

auto Velocity::speed() const noexcept -> Eigen::Vector3f
{
  return m_speed;
}

void Velocity::accelerate(const Eigen::Vector3f &acc)
{
  m_acceleration = acc;
}

void Velocity::update(const float elapsedSeconds)
{
  m_speed += m_acceleration * elapsedSeconds;

  constexpr auto FRICTION_PERCENTAGE = 0.01f;
  Eigen::Vector3f friction           = -FRICTION_PERCENTAGE * m_speed;
  m_speed += friction;

  constexpr auto SLOW_SPEED_STOP_THRESHOLD = 0.2f;
  if (m_acceleration.isZero() && m_speed.norm() < SLOW_SPEED_STOP_THRESHOLD)
  {
    m_speed = Eigen::Vector3f::Zero();
  }
}

} // namespace bsgo
