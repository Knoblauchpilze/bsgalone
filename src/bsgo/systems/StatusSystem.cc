
#include "StatusSystem.hh"
#include "Coordinator.hh"
#include "JumpMessage.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<StatusComponent>();
}
} // namespace

StatusSystem::StatusSystem()
  : AbstractSystem(SystemType::STATUS, isEntityRelevant)
{}

constexpr auto TIME_TO_STAY_IN_APPEARED_MODE = core::Milliseconds{10'000};
constexpr auto TIME_TO_STAY_IN_THREAT_MODE   = core::Milliseconds{3'000};

void StatusSystem::updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const
{
  auto &statusComp = entity.statusComp();
  statusComp.update(data);

  handleAppearingState(entity, statusComp);
  handleThreatState(entity, statusComp);
  handleJustChangedState(entity, statusComp);
  handleJumpState(entity, statusComp, coordinator);
}

void StatusSystem::handleAppearingState(Entity &entity, StatusComponent &statusComp) const
{
  const auto status     = statusComp.status();
  const auto lastUpdate = statusComp.tryGetElapsedSinceLastAppearing();
  if (statusIndicatesAppearing(status) && lastUpdate && *lastUpdate >= TIME_TO_STAY_IN_APPEARED_MODE)
  {
    debug("Switching " + entity.str() + " from appearing to visible");
    statusComp.setStatus(updateStatusAfterSpawn(status));
    entity.tryMarkForNetworkSync();
  }
}

void StatusSystem::handleThreatState(Entity &entity, StatusComponent &statusComp) const
{
  const auto status     = statusComp.status();
  const auto lastUpdate = statusComp.getElapsedSinceLastChange();
  if (statusIndicatesThreat(status) && lastUpdate >= TIME_TO_STAY_IN_THREAT_MODE)
  {
    debug(entity.str() + " is no more in threat");
    statusComp.setStatus(updateStatusAfterThreatEnded(status));
    entity.tryMarkForNetworkSync();

    if (entity.exists<LootComponent>())
    {
      entity.lootComp().clearRecipients();
    }
  }
}

void StatusSystem::handleJustChangedState(Entity &entity, StatusComponent &statusComp) const
{
  if (!statusComp.justChanged())
  {
    return;
  }

  const auto status = statusComp.status();
  if (statusRequiresImmobilization(status) && entity.exists<VelocityComponent>())
  {
    auto &velocity = entity.velocityComp();
    velocity.immobilize();
  }
  if (statusRequiresThreatReset(status))
  {
    statusComp.setStatus(updateStatusAfterThreatEnded(status));
  }
  if (statusRequiresPowerReset(status) && entity.exists<PowerComponent>())
  {
    auto &power = entity.powerComp();
    power.useAll();
  }

  entity.tryMarkForNetworkSync();
  statusComp.resetChanged();
}

void StatusSystem::handleJumpState(Entity &entity,
                                   StatusComponent &statusComp,
                                   Coordinator &coordinator) const
{
  const auto status = statusComp.status();
  if (!statusIndicatesJump(status))
  {
    return;
  }

  const auto remaining = statusComp.tryGetRemainingJumpTime();

  if (remaining > core::Duration{0})
  {
    return;
  }

  const auto owner       = entity.ownerComp().owner();
  const auto ownerEntity = coordinator.getEntity(owner);

  const auto shipDbId   = entity.dbComp().dbId();
  const auto playerDbId = ownerEntity.dbComp().dbId();

  // Reset the status to not trigger more jump messages.
  statusComp.setStatus(Status::SPECTATOR);

  pushInternalMessage(std::make_unique<JumpMessage>(shipDbId, playerDbId));
  entity.tryMarkForNetworkSync();
}

} // namespace bsgo
