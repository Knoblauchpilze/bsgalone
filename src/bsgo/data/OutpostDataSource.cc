
#include "OutpostDataSource.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"

namespace bsgo {

OutpostDataSource::OutpostDataSource(const Repositories &repositories)
  : core::CoreObject("bsgo")
  , m_repositories(repositories)
{
  setService("data");
  addModule("outpost");
}

void OutpostDataSource::initialize(const Uuid systemDbId,
                                   Coordinator &coordinator,
                                   DatabaseEntityMapper &entityMapper) const
{
  const auto outposts = m_repositories.systemRepository->findAllOutpostsBySystem(systemDbId);
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
  coordinator.addNetwork(outpostEntityId, {ComponentType::HEALTH, ComponentType::POWER});
  coordinator.addStatus(outpostEntityId, Status::VISIBLE, {}, {});

  entityMapper.registerOutpost(outpostDbId, outpostEntityId);
}

} // namespace bsgo
