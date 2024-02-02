
#include "DataSource.hh"
#include "AsteroidDataSource.hh"
#include "Coordinator.hh"
#include "OutpostDataSource.hh"
#include "PlayerDataSource.hh"
#include "ShipDataSource.hh"

namespace bsgo {

DataSource::DataSource(const DataLoadingMode dataLoadingMode)
  : utils::CoreObject("bsgo")
  , m_dataLoadingMode(dataLoadingMode)
{
  setService("data");
  m_connection->connect();
  m_repositories = createRepositories(m_connection);
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
  m_playerDbId = player;
}

void DataSource::clearSystemDbId()
{
  m_systemDbId.reset();
}

auto DataSource::playerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
}

auto DataSource::repositories() const -> Repositories
{
  return m_repositories;
}

void DataSource::initialize(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const
{
  if (!m_systemDbId)
  {
    if (!m_playerDbId)
    {
      error("Failed to initialize the game", "No system not player id defined");
    }

    m_systemDbId = m_repositories.playerRepository->findSystemByPlayer(*m_playerDbId);
  }

  coordinator.clear();
  entityMapper.clear();
  if (m_playerDbId)
  {
    entityMapper.setPlayerDbId(*m_playerDbId);
  }

  initializePlayer(coordinator, entityMapper);
  initializeShips(coordinator, entityMapper);
  initializeAsteroids(coordinator, entityMapper);
  initializeOutposts(coordinator);
}

void DataSource::initializePlayer(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const
{
  PlayerDataSource source(m_repositories, *m_systemDbId);
  source.initialize(coordinator, entityMapper);

  if (DataLoadingMode::CLIENT == m_dataLoadingMode
      && !entityMapper.tryGetPlayerEntityId().has_value())
  {
    error("Failed to initialize player", "Could not find entity id for " + str(*m_playerDbId));
  }
}

void DataSource::initializeAsteroids(Coordinator &coordinator,
                                     DatabaseEntityMapper &entityMapper) const
{
  AsteroidDataSource source(m_repositories, *m_systemDbId);
  source.initialize(coordinator, entityMapper);
}

void DataSource::initializeShips(Coordinator &coordinator, DatabaseEntityMapper &entityMapper) const
{
  ShipDataSource source(m_repositories, *m_systemDbId);
  source.initialize(coordinator, entityMapper);
}

void DataSource::initializeOutposts(Coordinator &coordinator) const
{
  OutpostDataSource source(m_repositories, *m_systemDbId);
  source.initialize(coordinator);
}

} // namespace bsgo
