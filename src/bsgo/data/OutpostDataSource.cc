
#include "OutpostDataSource.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"

namespace bsgo {

OutpostDataSource::OutpostDataSource(const Repositories &repositories, const Uuid systemDbId)
  : utils::CoreObject("bsgo")
  , m_systemDbId(systemDbId)
  , m_repositories(repositories)
{
  setService("data");
  addModule("outpost");
}

void OutpostDataSource::initialize(Coordinator &coordinator) const
{
  const auto outposts = m_repositories.systemRepository->findAllOutpostsBySystem(m_systemDbId);
  for (const auto &id : outposts)
  {
    registerOutpost(coordinator, id);
  }
}

void OutpostDataSource::registerOutpost(Coordinator &coordinator, const Uuid outpost) const
{
  const auto data = m_repositories.systemOutpostRepository->findOneById(outpost);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::OUTPOST);
  coordinator.addDbId(ent, outpost);
  coordinator.addTransform(ent, std::move(box));
  coordinator.addHealth(ent, data.hullPoints, data.maxHullPoints, data.hullPointsRegen);
  coordinator.addRemoval(ent);
  coordinator.addPower(ent, data.powerPoints, data.maxPowerPoints, data.powerRegen);
  coordinator.addTarget(ent);
  coordinator.addFaction(ent, data.faction);
  coordinator.addStatus(ent, Status::VISIBLE, {}, {});
}

} // namespace bsgo
