
#include "DataSource.hh"
#include "AsteroidDataSource.hh"
#include "Coordinator.hh"
#include "OutpostDataSource.hh"
#include "PlayerDataSource.hh"
#include "ShipDataSource.hh"

namespace bsgo {

DataSource::DataSource(const DataLoadingMode dataLoadingMode)
  : core::CoreObject("bsgo")
  , m_dataLoadingMode(dataLoadingMode)
{
  setService("data");
}

void DataSource::setSystemDbId(const Uuid system)
{
  if (DataLoadingMode::CLIENT == m_dataLoadingMode)
  {
    error("Unable to set system id to " + str(system), "Unavailable in client mode");
  }
  m_systemDbId = system;
}

void DataSource::setPlayerDbId(const Uuid player)
{
  if (DataLoadingMode::SERVER == m_dataLoadingMode)
  {
    error("Unable to set player id to " + str(player), "Unavailable in server mode");
  }
  m_systemDbId.reset();
}

void DataSource::clearSystemDbId()
{
  m_systemDbId.reset();
}

auto DataSource::repositories() const -> Repositories
{
  return m_repositories;
}

void DataSource::initialize(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const
{
  if (m_dataLoadingMode == DataLoadingMode::CLIENT)
  {
    error("Initialization from the data source is not supported in client mode");
  }
  if (!m_systemDbId)
  {
    error("Failed to initialize the game", "No system id defined");
  }

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
  source.initialize(*m_systemDbId, coordinator, entityMapper);
}

void DataSource::initializeAsteroids(Coordinator &coordinator,
                                     DatabaseEntityMapper &entityMapper) const
{
  AsteroidDataSource source{m_repositories};
  source.initialize(*m_systemDbId, coordinator, entityMapper);
}

void DataSource::initializeShips(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const
{
  ShipDataSource source{m_repositories};
  source.initialize(*m_systemDbId, coordinator, entityMapper);
}

void DataSource::initializeOutposts(Coordinator &coordinator,
                                    DatabaseEntityMapper &entityMapper) const
{
  OutpostDataSource source{m_repositories};
  source.initialize(*m_systemDbId, coordinator, entityMapper);
}

} // namespace bsgo
