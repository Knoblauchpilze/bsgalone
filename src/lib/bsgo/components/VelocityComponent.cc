
#include "VelocityComponent.hh"

#include "VectorUtils.hh"

namespace bsgo {
constexpr auto FRICTION_ACCELERATION = 0.5f;

VelocityComponent::VelocityComponent(const VelocityData &data)
  : IComponent("velocity")
  , m_speedMode(data.speedMode)
  , m_maxAcceleration(data.maxAcceleration)
  , m_maxSpeed(data.maxSpeed)
{
  if (data.initialSpeed)
  {
    m_speed = *data.initialSpeed;
  }
}

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

void VelocityComponent::setSpeed(const Eigen::Vector3f &direction)
{
  const Eigen::Vector3f nDir = direction.normalized();
  m_speed                    = nDir * m_maxSpeed;
}

void VelocityComponent::update(const float elapsedSeconds)
{
  switch (m_speedMode)
  {
    case SpeedMode::FIXED:
      updateFixedSpeed(elapsedSeconds);
      break;
    case SpeedMode::VARIABLE:
      updateVariableSpeed(elapsedSeconds);
      break;
    default:
      error("Failed to updated velocity",
            "Unsupported speed mode " + std::to_string(static_cast<int>(m_speedMode)));
      break;
  }
}

void VelocityComponent::updateFixedSpeed(const float /*elapsedSeconds*/)
{
  // log("speed before friction is " + str(m_speed) + " due to accel " + str(m_acceleration));
  // log("speed after friction is " + str(m_speed) + " due to friction " + str(friction));
  // log("speed after clamp is " + str(m_speed) + " due to max speed " + std::to_string(m_maxSpeed));
}

void VelocityComponent::updateVariableSpeed(const float elapsedSeconds)
{
  /// https://gamedev.stackexchange.com/questions/69404/how-should-i-implement-basic-spaceship-physics
  m_speed += m_acceleration * elapsedSeconds;

  Eigen::Vector3f friction = -FRICTION_ACCELERATION * elapsedSeconds * m_speed.normalized();
  m_speed += friction;

  const auto speedNorm = m_speed.norm();
  if (speedNorm >= m_maxSpeed)
  {
    m_speed.normalize();
    m_speed *= m_maxSpeed;
  }

  constexpr auto SLOW_SPEED_STOP_THRESHOLD = 0.2f;
  if (m_acceleration.isZero() && m_speed.norm() < SLOW_SPEED_STOP_THRESHOLD)
  {
    m_speed = Eigen::Vector3f::Zero();
  }
}

} // namespace bsgo
