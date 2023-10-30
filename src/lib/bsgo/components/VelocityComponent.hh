
#pragma once

#include "IComponent.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class VelocityComponent : public IComponent
{
  public:
  VelocityComponent(const float maxAcceleration);
  VelocityComponent(const Eigen::Vector3f &speed);
  ~VelocityComponent() override = default;

  auto acceleration() const noexcept -> Eigen::Vector3f;
  auto speed() const noexcept -> Eigen::Vector3f;

  /// @brief - Set the maximum acceleration directed in the input direction for
  /// this component.
  /// @param direction - the direction to accelerate into.
  void accelerate(const Eigen::Vector3f &direction);

  void update(const float elapsedSeconds) override;

  private:
  float m_maxAcceleration;
  Eigen::Vector3f m_acceleration{Eigen::Vector3f::Zero()};
  Eigen::Vector3f m_speed{Eigen::Vector3f::Zero()};
};

using VelocityComponentShPtr = std::shared_ptr<VelocityComponent>;

} // namespace bsgo
