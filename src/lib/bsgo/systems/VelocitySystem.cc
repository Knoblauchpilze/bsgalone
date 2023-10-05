
#include "VelocitySystem.hh"

namespace bsgo {

VelocitySystem::VelocitySystem()
  : utils::CoreObject("velocity")
{
  setService("system");
}

void VelocitySystem::addVelocity(const Uuid &ent, const Eigen::Vector3f &speed)
{
  if (m_velocities.contains(ent))
  {
    warn("Overriding velocity for entity " + std::to_string(ent));
  }

  m_velocities[ent] = std::make_shared<Velocity>(speed);
}

auto VelocitySystem::getVelocity(const Uuid &ent) const -> std::optional<VelocityShPtr>
{
  const auto it = m_velocities.find(ent);
  if (m_velocities.end() == it)
  {
    return {};
  }

  return {it->second};
}

} // namespace bsgo
