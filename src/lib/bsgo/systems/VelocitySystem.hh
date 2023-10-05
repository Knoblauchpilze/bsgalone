
#pragma once

#include "Uuid.hh"
#include "Velocity.hh"
#include <core_utils/CoreObject.hh>
#include <eigen3/Eigen/Eigen>
#include <optional>
#include <unordered_map>

namespace bsgo {

class VelocitySystem : public utils::CoreObject
{
  public:
  VelocitySystem();

  void addVelocity(const Uuid &ent, const Eigen::Vector3f &speed);
  auto getVelocity(const Uuid &ent) const -> std::optional<VelocityShPtr>;

  private:
  std::unordered_map<Uuid, VelocityShPtr> m_velocities{};
};

} // namespace bsgo
