
#pragma once

#include "IComponent.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

enum class SpeedMode
{
  FIXED,
  VARIABLE
};

struct VelocityData
{
  float maxAcceleration{1.0f};
  float maxSpeed{1.0f};

  std::optional<Eigen::Vector3f> initialSpeed{};

  SpeedMode speedMode{SpeedMode::VARIABLE};
};

class VelocityComponent : public IComponent
{
  public:
  VelocityComponent(const VelocityData &data);
  ~VelocityComponent() override = default;

  auto acceleration() const noexcept -> Eigen::Vector3f;
  auto speed() const noexcept -> Eigen::Vector3f;

  /// @brief - Set the acceleration to its maximum allowed value in a direction
  /// equal to the input argument.
  /// @param direction - the direction to accelerate into.
  void accelerate(const Eigen::Vector3f &direction);

  /// @brief - Set the speed to its maximum allowed value in a direction equal to
  /// the input argument.
  /// @param direction - the direction towards which the speed points to.
  void setSpeed(const Eigen::Vector3f &direction);

  void immobilize();

  void update(const float elapsedSeconds) override;

  private:
  SpeedMode m_speedMode;
  float m_maxAcceleration;
  float m_maxSpeed;

  Eigen::Vector3f m_acceleration{Eigen::Vector3f::Zero()};
  Eigen::Vector3f m_speed{Eigen::Vector3f::Zero()};

  void updateFixedSpeed(const float elapsedSeconds);
  void updateVariableSpeed(const float elapsedSeconds);
};

using VelocityComponentShPtr = std::shared_ptr<VelocityComponent>;

} // namespace bsgo
