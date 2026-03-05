
#include "DataSource.hh"
#include "AsteroidDataSource.hh"
#include "Coordinator.hh"
#include "OutpostDataSource.hh"
#include "PlayerDataSource.hh"
#include "ShipDataSource.hh"

namespace bsgo {

DataSource::DataSource(const Uuid systemDbId)
  : core::CoreObject("bsgo")
  , m_systemDbId(systemDbId)
{
  setService("data");
}

void DataSource::initialize(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const
{
  coordinator.clear();
  entityMapper.clearAll();

  initializePlayers(coordinator, entityMapper);
  initializeShips(coordinator, entityMapper);
  initializeAsteroids(coordinator, entityMapper);
  initializeOutposts(coordinator, entityMapper);
}

void DataSource::initializePlayers(Coordinator &coordinator,
                                   DatabaseEntityMapper &entityMapper) const
{
  PlayerDataSource source{m_repositories};
  source.initialize(m_systemDbId, coordinator, entityMapper);
}

void DataSource::initializeAsteroids(Coordinator &coordinator,
                                     DatabaseEntityMapper &entityMapper) const
{
  AsteroidDataSource source{m_repositories};
  source.initialize(m_systemDbId, coordinator, entityMapper);
}

void DataSource::initializeShips(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const
{
  ShipDataSource source{m_repositories};
  source.initialize(m_systemDbId, coordinator, entityMapper);
}

void DataSource::initializeOutposts(Coordinator &coordinator,
                                    DatabaseEntityMapper &entityMapper) const
{
  OutpostDataSource source{m_repositories};
  source.initialize(m_systemDbId, coordinator, entityMapper);
}

} // namespace bsgo
