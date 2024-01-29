
#include "ComputerSystem.hh"
#include "ScannedMessage.hh"
#include "SlotComponentMessage.hh"
#include "SystemUtils.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return !entity.computers.empty();
}

bool isValidTarget(const Entity &entity, const Entity &target, const ComputerSlotComponent &computer)
{
  const auto matchingTargetKind = !computer.allowedTargets()
                                  || hasEntityMatchingKind(target, *computer.allowedTargets());
  return matchingTargetKind && hasTargetDifferentFaction(entity, target);
}
} // namespace

ComputerSystem::ComputerSystem()
  : AbstractSystem(SystemType::COMPUTER, isEntityRelevant)
{}

void ComputerSystem::updateEntity(Entity &entity,
                                  Coordinator &coordinator,
                                  const float elapsedSeconds) const
{
  std::optional<Entity> targetEnt;
  const auto target = entity.targetComp().target();
  if (target)
  {
    targetEnt = coordinator.getEntity(*target);
  }

  const auto count = static_cast<int>(entity.computers.size());
  for (auto id = 0; id < count; ++id)
  {
    const auto &computer = entity.computers[id];
    updateComputer(entity, computer, targetEnt, elapsedSeconds);
    if (computer->hasFireRequest())
    {
      if (processFireRequest(entity, computer, targetEnt, coordinator))
      {
        sendComponentUpdatedMessage(entity, id, *computer, coordinator);
      }
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

  if (computer->isOffensive() && (!target.has_value() || !isValidTarget(ent, *target, *computer)))
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

bool ComputerSystem::processFireRequest(Entity &ent,
                                        const ComputerSlotComponentShPtr &computer,
                                        std::optional<Entity> &target,
                                        Coordinator &coordinator) const
{
  computer->clearFireRequest();

  if (!computer->canFire())
  {
    return false;
  }

  computer->fire();
  const auto powerUsed = computer->powerCost();
  ent.powerComp().usePower(powerUsed);

  applyEmitterEffects(ent, computer, coordinator);
  if (computer->isOffensive() && target)
  {
    applyReceiverEffects(*target, computer, coordinator);
  }

  return true;
}

void ComputerSystem::applyEmitterEffects(Entity &ent,
                                         const ComputerSlotComponentShPtr &computer,
                                         Coordinator &coordinator) const
{
  const auto damage = computer->damageModifier();
  if (damage)
  {
    const auto duration = computer->duration();
    if (!duration)
    {
      error("Failed to activate computer", "Expected slot to define a duration");
    }
    debug("Adding weapon effect for " + ent.str() + " with duration "
          + utils::durationToPrettyString(*duration, true));
    coordinator.addWeaponEffect(ent.uuid, *duration, *damage);
  }
}

void ComputerSystem::applyReceiverEffects(Entity &target,
                                          const ComputerSlotComponentShPtr & /*computer*/,
                                          Coordinator & /*coordinator*/) const
{
  if (target.exists<ScannedComponent>())
  {
    target.scannedComp().scan();
    pushMessage(std::make_unique<ScannedMessage>(target.uuid));
  }
}

void ComputerSystem::sendComponentUpdatedMessage(const Entity &entity,
                                                 const int slotIndex,
                                                 const SlotComponent &component,
                                                 const Coordinator & /*coordinator*/) const
{
  /// TODO: Use the player db id to find some sort of client id
  // std::optional<Uuid> playerDbId{};
  // if (entity.exists<OwnerComponent>())
  // {
  //   const auto ownerEntityId = entity.ownerComp().owner();
  //   const auto player        = coordinator.getEntity(ownerEntityId);

  //   if (player.exists<DbComponent>())
  //   {
  //     playerDbId = player.dbComp().dbId();
  //   }
  // }

  pushMessage(std::make_unique<SlotComponentMessage>(entity.uuid, slotIndex, component));
}

} // namespace bsgo
