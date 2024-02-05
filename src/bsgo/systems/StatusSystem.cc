
#include "StatusSystem.hh"
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

constexpr auto TIME_TO_STAY_IN_APPEARED_MODE = utils::Milliseconds{10'000};
constexpr auto TIME_TO_STAY_IN_THREAT_MODE   = utils::Milliseconds{3'000};

void StatusSystem::updateEntity(Entity &entity,
                                Coordinator & /*coordinator*/,
                                const float elapsedSeconds) const
{
  auto &statusComp = entity.statusComp();
  statusComp.update(elapsedSeconds);

  handleAppearingState(entity, statusComp);
  handleThreatState(entity, statusComp);
  handleJustChangedState(entity, statusComp);
  handleJumpState(entity, statusComp);
}

void StatusSystem::handleAppearingState(Entity &entity, StatusComponent &statusComp) const
{
  const auto status     = statusComp.status();
  const auto lastUpdate = statusComp.tryGetElapsedSinceLastAppearing();
  if (statusIndicatesAppearing(status) && lastUpdate && *lastUpdate >= TIME_TO_STAY_IN_APPEARED_MODE)
  {
    debug("Switching " + entity.str() + " from appearing to visible");
    statusComp.setStatus(updateStatusAfterSpawn(status));
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

  statusComp.resetChanged();
}

void StatusSystem::handleJumpState(Entity &entity, StatusComponent &statusComp) const
{
  const auto status = statusComp.status();
  if (!statusIndicatesJump(status))
  {
    return;
  }

  const auto remaining = statusComp.tryGetRemainingJumpTime();
  if (remaining >= utils::Duration{0})
  {
    return;
  }

  auto message = std::make_unique<JumpMessage>(entity.dbComp().dbId());
  pushMessage(std::move(message));
}

} // namespace bsgo
