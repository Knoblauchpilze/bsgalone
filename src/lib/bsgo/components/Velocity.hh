
#pragma once

#include "IComponent.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class Velocity : public IComponent
{
  public:
  Velocity(const float maxAcceleration);
  ~Velocity() override = default;

  auto acceleration() const noexcept -> Eigen::Vector3f;
  auto speed() const noexcept -> Eigen::Vector3f;

  /// @brief - Sets the maximum acceleration directed in the input direction for
  /// this component.
  /// @param direction - the direction to accelerate into.
  void accelerate(const Eigen::Vector3f &direction);

  void update(const float elapsedSeconds) override;

  private:
  float m_maxAcceleration;
  Eigen::Vector3f m_acceleration{Eigen::Vector3f::Zero()};
  Eigen::Vector3f m_speed{Eigen::Vector3f::Zero()};
};

using VelocityShPtr = std::shared_ptr<Velocity>;

} // namespace bsgo
