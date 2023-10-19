
#include "MotionSystem.hh"

namespace bsgo {

MotionSystem::MotionSystem()
  : ISystem("motion")
{}

void MotionSystem::update(const Components &components,
                          const Coordinator & /*coordinator*/,
                          const float elapsedSeconds)
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

    const Eigen::Vector3f speed = vel->second->speed();
    Eigen::Vector3f dv          = speed * elapsedSeconds;
    transform->translate(dv);
    if (!speed.isZero())
    {
      transform->setHeading(std::atan2(speed(0), speed(1)));
    }
  }
}

} // namespace bsgo
