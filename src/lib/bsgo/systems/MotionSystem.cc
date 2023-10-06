
#include "MotionSystem.hh"

namespace bsgo {

MotionSystem::MotionSystem()
  : utils::CoreObject("motion")
{
  setService("system");
}

void MotionSystem::update(const Components &components, const float elapsedSeconds)
{
  for (const auto &[_, velocity] : components.velocities)
  {
    velocity->update(elapsedSeconds);
  }

  for (const auto &[uuid, transform] : components.transforms)
  {
    const auto vel = components.velocities.find(uuid);
    if (components.velocities.end() == vel)
    {
      continue;
    }

    Eigen::Vector3f dv = vel->second->speed() * elapsedSeconds;
    transform->translate(dv);
  }
}

} // namespace bsgo
