
#pragma once

#include "AbstractComponent.hh"
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

class VelocityComponent : public AbstractComponent
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

  /// @brief - Override the existing acceleration with the provided value without
  /// performing any checks.
  /// @param acceleration - the acceleration to set.
  void overrideAcceleration(const Eigen::Vector3f &acceleration);

  /// @brief - Set the speed to its maximum allowed value in a direction equal to
  /// the input argument.
  /// @param direction - the direction towards which the speed points to.
  void setSpeed(const Eigen::Vector3f &direction);

  /// @brief - Override the existing speed with the provided value without
  /// performing any checks.
  /// @param speed - the speed to set.
  void overrideSpeed(const Eigen::Vector3f &speed);

  void immobilize();

  void update(const TickData &data) override;

  private:
  SpeedMode m_speedMode;
  float m_maxAcceleration;
  float m_maxSpeed;

  Eigen::Vector3f m_acceleration{Eigen::Vector3f::Zero()};
  Eigen::Vector3f m_speed{Eigen::Vector3f::Zero()};

  void updateFixedSpeed(const TickData &data);
  void updateVariableSpeed(const TickData &data);
};

using VelocityComponentShPtr = std::shared_ptr<VelocityComponent>;

} // namespace bsgo
