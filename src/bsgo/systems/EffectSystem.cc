
#include "EffectSystem.hh"
#include "Coordinator.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return !entity.effects.empty();
}
} // namespace

EffectSystem::EffectSystem()
  : AbstractSystem(SystemType::EFFECT, isEntityRelevant)
{}

void EffectSystem::updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const
{
  for (const auto &effect : entity.effects)
  {
    // TODO: We should use the tick duration as is.
    effect->update(data.elapsed.toSeconds());
  }

  cleanUpFinishedEffects(entity, coordinator);
}

void EffectSystem::cleanUpFinishedEffects(const Entity &entity, Coordinator &coordinator) const
{
  std::vector<EffectComponentShPtr> toRemove;

  for (const auto &effect : entity.effects)
  {
    if (effect->isFinished())
    {
      toRemove.push_back(effect);
    }
  }

  for (const auto &finished : toRemove)
  {
    coordinator.removeEffect(entity.uuid, finished);
  }
}

} // namespace bsgo
