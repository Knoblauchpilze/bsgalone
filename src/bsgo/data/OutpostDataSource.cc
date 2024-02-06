
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

void OutpostDataSource::initialize(Coordinator &coordinator,
                                   DatabaseEntityMapper &entityMapper) const
{
  const auto outposts = m_repositories.systemRepository->findAllOutpostsBySystem(m_systemDbId);
  for (const auto &id : outposts)
  {
    registerOutpost(coordinator, id, entityMapper);
  }
}

void OutpostDataSource::registerOutpost(Coordinator &coordinator,
                                        const Uuid outpostDbId,
                                        DatabaseEntityMapper &entityMapper) const
{
  const auto data = m_repositories.systemOutpostRepository->findOneById(outpostDbId);

  auto box                   = std::make_unique<CircleBox>(data.position, data.radius);
  const auto outpostEntityId = coordinator.createEntity(EntityKind::OUTPOST);
  coordinator.addDbId(outpostEntityId, outpostDbId);
  coordinator.addTransform(outpostEntityId, std::move(box));
  coordinator.addHealth(outpostEntityId, data.hullPoints, data.maxHullPoints, data.hullPointsRegen);
  coordinator.addRemoval(outpostEntityId);
  coordinator.addPower(outpostEntityId, data.powerPoints, data.maxPowerPoints, data.powerRegen);
  coordinator.addTarget(outpostEntityId);
  coordinator.addFaction(outpostEntityId, data.faction);
  coordinator.addStatus(outpostEntityId, Status::VISIBLE, {}, {});

  entityMapper.registerOutpost(outpostDbId, outpostEntityId);
}

} // namespace bsgo
