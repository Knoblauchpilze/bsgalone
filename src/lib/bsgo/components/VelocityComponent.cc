
#include "VelocityComponent.hh"

namespace bsgo {
constexpr auto FRICTION_ACCELERATION = 0.5f;

VelocityComponent::VelocityComponent(const float maxAcceleration)
  : IComponent("velocity")
  , m_maxAcceleration(maxAcceleration)
{}

VelocityComponent::VelocityComponent(const Eigen::Vector3f &speed)
  : IComponent("velocity")
  , m_maxAcceleration(FRICTION_ACCELERATION)
  , m_speed(speed)
{}

auto VelocityComponent::acceleration() const noexcept -> Eigen::Vector3f
{
  return m_acceleration;
}

auto VelocityComponent::speed() const noexcept -> Eigen::Vector3f
{
  return m_speed;
}

void VelocityComponent::accelerate(const Eigen::Vector3f &direction)
{
  const Eigen::Vector3f nDir = direction.normalized();
  m_acceleration             = nDir * m_maxAcceleration;
}

void VelocityComponent::update(const float elapsedSeconds)
{
  m_speed += m_acceleration * elapsedSeconds;

  Eigen::Vector3f friction = -FRICTION_ACCELERATION * elapsedSeconds * m_speed.normalized();
  m_speed += friction;

  constexpr auto SLOW_SPEED_STOP_THRESHOLD = 0.2f;
  if (m_acceleration.isZero() && m_speed.norm() < SLOW_SPEED_STOP_THRESHOLD)
  {
    m_speed = Eigen::Vector3f::Zero();
  }
}

} // namespace bsgo
