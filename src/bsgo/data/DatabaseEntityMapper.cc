
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
  debug("registered ship " + str(shipDbId) + " for player " + str(playerDbId)
        + ", (player: " + (m_playerDbId ? str(*m_playerDbId) : "haha") + ")");
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

auto DatabaseEntityMapper::tryGetPlayerEntityId() const -> std::optional<Uuid>
{
  return m_playerEntityId;
}

auto DatabaseEntityMapper::playerEntityId() const -> Uuid
{
  if (!m_playerEntityId)
  {
    error("Expected to have a player entity id");
  }
  return *m_playerEntityId;
}

auto DatabaseEntityMapper::playerShipDbId() const -> Uuid
{
  if (!m_playerShipDbId)
  {
    error("Expected to have a player ship id");
  }
  return *m_playerShipDbId;
}

auto DatabaseEntityMapper::playerShipEntityId() const -> Uuid
{
  if (!m_playerShipEntityId)
  {
    error("Expected to have a player ship entity id");
  }
  return *m_playerShipEntityId;
}

auto DatabaseEntityMapper::tryGetPlayerEntityId(const Uuid playerDbId) const -> std::optional<Uuid>
{
  const auto maybePlayer = m_playerDbIdsToEntityIds.find(playerDbId);
  if (maybePlayer != m_playerDbIdsToEntityIds.cend())
  {
    return maybePlayer->second;
  }

  return {};
}

auto DatabaseEntityMapper::tryGetShipEntityId(const Uuid shipDbId) const -> std::optional<Uuid>
{
  const auto maybeShip = m_shipDbIdsToEntityIds.find(shipDbId);
  if (maybeShip != m_shipDbIdsToEntityIds.cend())
  {
    return maybeShip->second;
  }

  return {};
}

void DatabaseEntityMapper::clear()
{
  m_playerDbIdsToEntityIds.clear();
  m_shipDbIdsToEntityIds.clear();

  m_playerDbId.reset();
  m_playerEntityId.reset();
  m_playerShipDbId.reset();
  m_playerShipEntityId.reset();
}

} // namespace bsgo
