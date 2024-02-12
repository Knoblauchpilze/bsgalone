
#include "DatabaseEntityMapper.hh"

namespace bsgo {

DatabaseEntityMapper::DatabaseEntityMapper()
  : utils::CoreObject("database")
{
  setService("mapper");
}

void DatabaseEntityMapper::setPlayerDbId(const Uuid playerDbId)
{
  m_playerDbId = playerDbId;
}

void DatabaseEntityMapper::registerPlayer(const Uuid playerDbId, const Uuid entityId)
{
  const std::lock_guard guard(m_locker);

  const auto res = m_playerDbIdsToEntityIds.try_emplace(playerDbId, entityId);
  if (!res.second)
  {
    error("Unable to register player " + str(playerDbId),
          "Player already is attached to entity " + str(res.first->second));
  }

  if (m_playerDbId && *m_playerDbId == playerDbId)
  {
    if (m_playerEntityId)
    {
      error("Failed to attach " + str(*m_playerDbId) + " to entity " + str(entityId),
            "Player is already attached to entity " + str(*m_playerEntityId));
    }
    m_playerEntityId = entityId;
  }
}

void DatabaseEntityMapper::registerShip(const Uuid shipDbId, const Uuid entityId)
{
  const std::lock_guard guard(m_locker);

  const auto res = m_shipDbIdsToEntityIds.try_emplace(shipDbId, entityId);
  if (!res.second)
  {
    error("Unable to register ship " + str(shipDbId),
          "Ship already is attached to entity " + str(res.first->second));
  }
}

void DatabaseEntityMapper::registerShipForPlayer(const Uuid playerDbId,
                                                 const Uuid shipDbId,
                                                 const Uuid entityId)
{
  registerShip(shipDbId, entityId);
  if (m_playerDbId && *m_playerDbId == playerDbId)
  {
    if (m_playerShipDbId)
    {
      error("Failed to attach " + str(*m_playerDbId) + " to ship " + str(shipDbId),
            "Player is already attached to entity " + str(*m_playerShipDbId));
    }
    m_playerShipDbId = shipDbId;

    if (m_playerShipEntityId)
    {
      error("Failed to attach " + str(*m_playerDbId) + " to ship entity " + str(entityId),
            "Player is already attached to entity " + str(*m_playerShipEntityId));
    }
    m_playerShipEntityId = entityId;
  }
}

void DatabaseEntityMapper::registerAsteroid(const Uuid asteroidDbId, const Uuid entityId)
{
  const std::lock_guard guard(m_locker);

  const auto res = m_asteroidDbIdsToEntityIds.try_emplace(asteroidDbId, entityId);
  if (!res.second)
  {
    error("Unable to register asteroid " + str(asteroidDbId),
          "Asteroid already is attached to entity " + str(res.first->second));
  }
}

void DatabaseEntityMapper::registerOutpost(const Uuid outpostDbId, const Uuid entityId)
{
  const std::lock_guard guard(m_locker);

  const auto res = m_outpostDbIdsToEntityIds.try_emplace(outpostDbId, entityId);
  if (!res.second)
  {
    error("Unable to register outpost " + str(outpostDbId),
          "Outpost already is attached to entity " + str(res.first->second));
  }
}

bool DatabaseEntityMapper::tryRemoveEntityForShip(const Uuid shipDbId)
{
  const std::lock_guard guard(m_locker);
  return m_shipDbIdsToEntityIds.erase(shipDbId);
}

void DatabaseEntityMapper::removeEntityForShip(const Uuid shipDbId)
{
  const std::lock_guard guard(m_locker);

  const auto res = m_shipDbIdsToEntityIds.erase(shipDbId);
  if (res != 1)
  {
    error("Unable to remove entity for ship " + str(shipDbId),
          "Removed " + std::to_string(res) + " entity(ies)");
  }
}

void DatabaseEntityMapper::removeEntityForAsteroid(const Uuid asteroidDbId)
{
  const std::lock_guard guard(m_locker);

  const auto res = m_asteroidDbIdsToEntityIds.erase(asteroidDbId);
  if (res != 1)
  {
    error("Unable to remove entity for asteroid " + str(asteroidDbId),
          "Removed " + std::to_string(res) + " entity(ies)");
  }
}

auto DatabaseEntityMapper::tryGetPlayerEntityId() const -> std::optional<Uuid>
{
  return m_playerEntityId;
}

auto DatabaseEntityMapper::tryGetPlayerShipDbId() const -> std::optional<Uuid>
{
  return m_playerShipDbId;
}

auto DatabaseEntityMapper::tryGetPlayerShipEntityId() const -> std::optional<Uuid>
{
  return m_playerShipEntityId;
}

auto DatabaseEntityMapper::tryGetPlayerEntityId(const Uuid playerDbId) const -> std::optional<Uuid>
{
  const std::lock_guard guard(m_locker);

  const auto maybePlayer = m_playerDbIdsToEntityIds.find(playerDbId);
  if (maybePlayer != m_playerDbIdsToEntityIds.cend())
  {
    return maybePlayer->second;
  }

  return {};
}

auto DatabaseEntityMapper::tryGetShipEntityId(const Uuid shipDbId) const -> std::optional<Uuid>
{
  const std::lock_guard guard(m_locker);

  const auto maybeShip = m_shipDbIdsToEntityIds.find(shipDbId);
  if (maybeShip != m_shipDbIdsToEntityIds.cend())
  {
    return maybeShip->second;
  }

  return {};
}

auto DatabaseEntityMapper::tryGetAsteroidEntityId(const Uuid asteroidDbId) const
  -> std::optional<Uuid>
{
  const std::lock_guard guard(m_locker);

  const auto maybeAsteroid = m_asteroidDbIdsToEntityIds.find(asteroidDbId);
  if (maybeAsteroid != m_asteroidDbIdsToEntityIds.cend())
  {
    return maybeAsteroid->second;
  }

  return {};
}

auto DatabaseEntityMapper::tryGetOutpostEntityId(const Uuid outpostDbId) const
  -> std::optional<Uuid>
{
  const std::lock_guard guard(m_locker);

  const auto maybeOutpost = m_outpostDbIdsToEntityIds.find(outpostDbId);
  if (maybeOutpost != m_outpostDbIdsToEntityIds.cend())
  {
    return maybeOutpost->second;
  }

  return {};
}

void DatabaseEntityMapper::clear()
{
  const std::lock_guard guard(m_locker);

  m_playerDbIdsToEntityIds.clear();
  m_shipDbIdsToEntityIds.clear();
  m_asteroidDbIdsToEntityIds.clear();
  m_outpostDbIdsToEntityIds.clear();

  m_playerDbId.reset();
  m_playerEntityId.reset();
  m_playerShipDbId.reset();
  m_playerShipEntityId.reset();
}

} // namespace bsgo
