
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

  info("setting player db id to " + str(player));
  m_playerDbId = player;
  m_systemDbId.reset();
}

void DataSource::clearSystemDbId()
{
  m_systemDbId.reset();
}

auto DataSource::tryGetPlayerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
}

auto DataSource::tryGetPlayerShipDbId() const -> std::optional<Uuid>
{
  if (!m_playerDbId)
  {
    return {};
  }

  const auto playerShip = m_repositories.playerShipRepository->findOneByPlayerAndActive(
    *m_playerDbId);
  return playerShip.id;
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
      error("Failed to initialize the game", "No system nor player id defined");
    }

    m_systemDbId = m_repositories.playerRepository->findSystemByPlayer(*m_playerDbId);
    info("found system " + str(*m_systemDbId) + " for player " + str(*m_playerDbId));
  }

  if (DataLoadingMode::SERVER == m_dataLoadingMode)
  {
    coordinator.clear();
    entityMapper.clear();
    if (m_playerDbId)
    {
      entityMapper.setPlayerDbId(*m_playerDbId);
    }
  }

  info("loading data");
  initializePlayers(coordinator, entityMapper);
  initializeShips(coordinator, entityMapper);
  initializeAsteroids(coordinator, entityMapper);
  initializeOutposts(coordinator, entityMapper);
}

void DataSource::initializePlayers(Coordinator &coordinator,
                                   DatabaseEntityMapper &entityMapper) const
{
  if (DataLoadingMode::SERVER == m_dataLoadingMode)
  {
    PlayerDataSource source{m_repositories};
    source.initialize(*m_systemDbId, coordinator, entityMapper);
  }

  if (DataLoadingMode::CLIENT == m_dataLoadingMode
      && !entityMapper.tryGetPlayerEntityId().has_value())
  {
    error("Failed to initialize player", "Could not find entity id for " + str(*m_playerDbId));
  }
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
