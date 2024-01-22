
#include "DataSource.hh"
#include "AsteroidDataSource.hh"
#include "Coordinator.hh"
#include "OutpostDataSource.hh"
#include "PlayerDataSource.hh"
#include "ShipDataSource.hh"

namespace bsgo {

DataSource::DataSource()
  : utils::CoreObject("bsgo")
{
  setService("data");
  m_connection->connect();
  m_repositories = createRepositories(m_connection);
}

void DataSource::setSystemDbId(const Uuid &system)
{
  m_systemDbId = system;
}

void DataSource::setPlayerDbId(const Uuid &player)
{
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

auto DataSource::playerEntityId() const -> Uuid
{
  if (!m_playerEntityId)
  {
    error("Expected to have a player entity id");
  }
  return *m_playerEntityId;
}

auto DataSource::playerShipDbId() const -> Uuid
{
  if (!m_playerShipDbId)
  {
    error("Expected to have a player ship id");
  }
  return *m_playerShipDbId;
}

auto DataSource::playerShipEntityId() const -> Uuid
{
  if (!m_playerShipEntityId)
  {
    error("Expected to have a player ship entity id");
  }
  return *m_playerShipEntityId;
}

void DataSource::initialize(Coordinator &coordinator) const
{
  if (!m_systemDbId)
  {
    if (!m_playerDbId)
    {
      error("Failed to initialize the game", "No system not player id defined");
    }

    m_systemDbId = m_repositories.playerRepository->findSystemByPlayer(*m_playerDbId);
  }

  m_playerEntityId.reset();
  m_playerShipDbId.reset();
  m_playerShipEntityId.reset();

  coordinator.clear();

  if (m_playerDbId)
  {
    initializePlayer(coordinator);
  }
  initializeShips(coordinator);
  initializeAsteroids(coordinator);
  initializeOutposts(coordinator);
}

void DataSource::initializePlayer(Coordinator &coordinator) const
{
  PlayerDataSource source(m_repositories, *m_playerDbId);
  m_playerEntityId = source.initialize(coordinator);
}

void DataSource::initializeAsteroids(Coordinator &coordinator) const
{
  AsteroidDataSource source(m_repositories, *m_systemDbId);
  source.initialize(coordinator);
}

void DataSource::initializeShips(Coordinator &coordinator) const
{
  ShipDataSource source(m_repositories, *m_systemDbId, *m_playerDbId, *m_playerEntityId);
  source.initialize(coordinator);

  m_playerShipDbId     = source.getPlayerShipDbId();
  m_playerShipEntityId = source.getPlayerShipEntityId();
}

void DataSource::initializeOutposts(Coordinator &coordinator) const
{
  OutpostDataSource source(m_repositories, *m_systemDbId);
  source.initialize(coordinator);
}

} // namespace bsgo
