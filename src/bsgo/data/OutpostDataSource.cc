
#include "OutpostDataSource.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"

namespace bsgo {

OutpostDataSource::OutpostDataSource()
  : core::CoreObject("bsgo")
{
  setService("data");
  addModule("outpost");
}

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
  if (!m_repositories)
  {
    error("Failed to initialize asteroid", "Repositories are not set");
  }

  const auto outposts = m_repositories->systemRepository->findAllOutpostsBySystem(systemDbId);
  for (const auto &id : outposts)
  {
    registerOutpost(coordinator, id, entityMapper);
  }
}

void OutpostDataSource::registerOutpost(Coordinator &coordinator,
                                        const OutpostData &data,
                                        DatabaseEntityMapper &entityMapper) const
{
  const auto outpostEntityId = coordinator.createEntity(EntityKind::OUTPOST);

  coordinator.addDbId(outpostEntityId, data.dbId);
  auto box = std::make_unique<CircleBox>(data.position, data.radius);
  coordinator.addTransform(outpostEntityId, std::move(box));
  // TODO: We also need to add the max and regen values for hull and power
  // coordinator.addHealth(outpostEntityId, data.hullPoints, data.maxHullPoints, data.hullPointsRegen);
  coordinator.addHealth(outpostEntityId, data.hullPoints, data.hullPoints, data.hullPoints);
  coordinator.addRemoval(outpostEntityId);
  // coordinator.addPower(outpostEntityId, data.powerPoints, data.maxPowerPoints, data.powerRegen);
  coordinator.addPower(outpostEntityId, data.powerPoints, data.powerPoints, data.powerPoints);
  coordinator.addTarget(outpostEntityId);
  coordinator.addFaction(outpostEntityId, data.faction);
  coordinator.addNetwork(outpostEntityId, {ComponentType::HEALTH, ComponentType::POWER});
  coordinator.addStatus(outpostEntityId, Status::VISIBLE, {}, {});

  entityMapper.registerOutpost(data.dbId, outpostEntityId);
}

void OutpostDataSource::registerOutpost(Coordinator &coordinator,
                                        const Uuid outpostDbId,
                                        DatabaseEntityMapper &entityMapper) const
{
  const auto data = m_repositories->systemOutpostRepository->findOneById(outpostDbId);

  OutpostData out{
    .dbId        = outpostDbId,
    .position    = data.position,
    .radius      = data.radius,
    .hullPoints  = data.hullPoints,
    .powerPoints = data.powerPoints,
    .faction     = data.faction,
  };

  registerOutpost(coordinator, out, entityMapper);
}

} // namespace bsgo
