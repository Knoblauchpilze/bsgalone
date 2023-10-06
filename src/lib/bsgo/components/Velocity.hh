
#pragma once

#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

class Velocity
{
  public:
  Velocity() = default;
  Velocity(const Eigen::Vector3f &acceleration, const Eigen::Vector3f &speed);
  ~Velocity() = default;

  auto acceleration() const noexcept -> Eigen::Vector3f;
  auto speed() const noexcept -> Eigen::Vector3f;

  void accelerate(const Eigen::Vector3f &acc);
  void update(const float elapsedSeconds);

  private:
  Eigen::Vector3f m_acceleration{Eigen::Vector3f::Zero()};
  Eigen::Vector3f m_speed{Eigen::Vector3f::Zero()};
};

using VelocityShPtr = std::shared_ptr<Velocity>;

} // namespace bsgo
