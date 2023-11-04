
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

void StatusSystem::updateEntity(Entity &entity,
                                Coordinator & /*coordinator*/,
                                const float elapsedSeconds) const
{
  auto &statusComp = entity.statusComp();
  statusComp.update(elapsedSeconds);

  const auto lastUpdate = statusComp.getElapsedSinceLastChange();
  if (Status::APPEARING == statusComp.status() && lastUpdate >= TIME_TO_STAY_IN_APPEARED_MODE)
  {
    log("Switching " + entity.str() + " to visible");
    statusComp.setStatus(Status::VISIBLE);
  }
}

} // namespace bsgo
