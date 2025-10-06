
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
                                const TickData &data) const
{
  auto &velocity  = entity.velocityComp();
  auto &transform = entity.transformComp();

  velocity.update(data);

  const Eigen::Vector3f speed = velocity.speed();
  // TODO: We should use the tick duration as is.
  Eigen::Vector3f dv = speed * data.elapsed.toSeconds();
  transform.translate(dv);
  if (!speed.isZero())
  {
    transform.setHeading(std::atan2(speed(0), speed(1)));
  }
}

} // namespace bsgo
