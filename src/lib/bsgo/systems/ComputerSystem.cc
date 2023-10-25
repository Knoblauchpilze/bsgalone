
#include "ComputerSystem.hh"
#include "SystemUtils.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return !entity.computers.empty();
}
} // namespace

ComputerSystem::ComputerSystem()
  : AbstractSystem("computer", isEntityRelevant)
{}

void ComputerSystem::updateEntity(Entity &entity,
                                  Coordinator &coordinator,
                                  const float elapsedSeconds) const
{
  const auto target = entity.targetComp().target();
  std::optional<Entity> targetEnt;
  if (target)
  {
    targetEnt = coordinator.getEntity(*target);
  }

  for (const auto &computer : entity.computers)
  {
    updateComputer(entity, computer, targetEnt, elapsedSeconds);
    if (computer->hasFireRequest())
    {
      processFireRequest(entity, computer, coordinator);
    }
  }
}

void ComputerSystem::updateComputer(const Entity &ent,
                                    const ComputerSlotComponentShPtr &computer,
                                    const std::optional<Entity> &target,
                                    const float elapsedSeconds) const
{
  auto state{FiringState::READY};

  computer->update(elapsedSeconds);

  if (computer->isOffensive() && (!target.has_value() || !hasTargetDifferentFaction(ent, *target)))
  {
    state = FiringState::INVALID_TARGET;
  }
  else if (computer->isOffensive() && distanceToTarget(ent, *target) > computer->range())
  {
    state = FiringState::OUT_OF_RANGE;
  }
  else if (computer->powerCost() > ent.powerComp().value())
  {
    state = FiringState::OUT_OF_POWER;
  }
  else if (computer->isReloading())
  {
    state = FiringState::RELOADING;
  }

  computer->setFiringState(state);
}

void ComputerSystem::processFireRequest(Entity &ent,
                                        const ComputerSlotComponentShPtr &computer,
                                        Coordinator &coordinator) const
{
  computer->clearFireRequest();

  if (!computer->canFire())
  {
    return;
  }

  const auto damage = computer->damageModifier();
  if (damage)
  {
    const auto duration = computer->duration();
    if (!duration)
    {
      error("Failed to activate computer", "Expected slot to define a duration");
    }
    log("Adding weapon effect for " + ent.str() + " with duration "
        + utils::durationToString(*duration));
    coordinator.addWeaponEffect(ent.uuid, *duration, *damage);
  }

  computer->fire();
  const auto powerUsed = computer->powerCost();
  ent.powerComp().usePower(powerUsed);
}

} // namespace bsgo
