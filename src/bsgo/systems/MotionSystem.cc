
#include "MotionSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<TransformComponent>() && ent.exists<VelocityComponent>();
}
} // namespace

MotionSystem::MotionSystem()
  : AbstractSystem(SystemType::MOTION, isEntityRelevant)
{}

void MotionSystem::updateEntity(Entity &entity,
                                Coordinator & /*coordinator*/,
                                const float elapsedSeconds) const
{
  auto &velocity  = entity.velocityComp();
  auto &transform = entity.transformComp();

  velocity.update(elapsedSeconds);

  const Eigen::Vector3f speed = velocity.speed();
  Eigen::Vector3f dv          = speed * elapsedSeconds;
  transform.translate(dv);
  if (!speed.isZero())
  {
    transform.setHeading(std::atan2(speed(0), speed(1)));
  }
}

} // namespace bsgo
