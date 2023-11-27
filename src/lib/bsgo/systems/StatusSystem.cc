
#include "StatusSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<StatusComponent>();
}
} // namespace

StatusSystem::StatusSystem()
  : AbstractSystem("status", isEntityRelevant)
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
}

void StatusSystem::handleAppearingState(Entity &entity, StatusComponent &statusComp) const
{
  const auto status     = statusComp.status();
  const auto lastUpdate = statusComp.getElapsedSinceLastChange();
  if (statusIndicatesAppearing(status) && lastUpdate >= TIME_TO_STAY_IN_APPEARED_MODE)
  {
    log("Switching " + entity.str() + " from appeating to visible");
    statusComp.setStatus(updateStatusAfterSpawn(status));
  }
}

void StatusSystem::handleThreatState(Entity &entity, StatusComponent &statusComp) const
{
  const auto status     = statusComp.status();
  const auto lastUpdate = statusComp.getElapsedSinceLastChange();
  if (statusIndicatesThreat(status) && lastUpdate >= TIME_TO_STAY_IN_THREAT_MODE)
  {
    log(entity.str() + " is no more in threat");
    statusComp.setStatus(updateStatusAfterThreatEnded(status));
  }
}

void StatusSystem::handleJustChangedState(Entity &entity, StatusComponent &statusComp) const
{
  if (!statusComp.justChanged())
  {
    return;
  }

  if (statusRequiresImmobilization(statusComp.status()) && entity.exists<VelocityComponent>())
  {
    auto &velocity = entity.velocityComp();
    velocity.immobilize();
  }

  statusComp.resetChanged();
}

} // namespace bsgo
