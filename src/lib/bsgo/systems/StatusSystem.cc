
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
  const auto lastUpdate = statusComp.getElapsedSinceLastChange();
  if (Status::APPEARING == statusComp.status() && lastUpdate >= TIME_TO_STAY_IN_APPEARED_MODE)
  {
    log("Switching " + entity.str() + " from appeating to visible");
    statusComp.setStatus(Status::VISIBLE);
  }
}

void StatusSystem::handleThreatState(Entity &entity, StatusComponent &statusComp) const
{
  const auto lastUpdate = statusComp.getElapsedSinceLastChange();
  if (Status::THREAT == statusComp.status() && lastUpdate >= TIME_TO_STAY_IN_THREAT_MODE)
  {
    log("Switching " + entity.str() + " from threat to visible");
    statusComp.setStatus(Status::VISIBLE);
  }
}

void StatusSystem::handleJustChangedState(Entity &entity, StatusComponent &statusComp) const
{
  if (!statusComp.justChanged())
  {
    return;
  }

  const auto statusRequiresImmobilization = Status::DOCKED == statusComp.status()
                                            || Status::DEAD == statusComp.status();
  if (statusRequiresImmobilization && entity.exists<VelocityComponent>())
  {
    auto &velocity = entity.velocityComp();
    velocity.immobilize();
  }

  statusComp.resetChanged();
}

} // namespace bsgo
