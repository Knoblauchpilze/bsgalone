
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

  // TODO: We should not convert to a real time here.
  // The conversion is based on the fact that a tick is supposed to last
  // 100 ms. Note that 0.1 can't be accurately represented in binary so
  // this is losing precision.
  constexpr auto SECONDS_IN_A_TICK = 0.1f;
  const auto elapsedSeconds        = data.elapsed.elapsed() * SECONDS_IN_A_TICK;

  const Eigen::Vector3f speed = velocity.speed();
  Eigen::Vector3f dv          = speed * elapsedSeconds;
  transform.translate(dv);
  if (!speed.isZero())
  {
    transform.setHeading(std::atan2(speed(0), speed(1)));
  }
}

} // namespace bsgo
