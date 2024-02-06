
#pragma once

#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace bsgo {

using PlayerDbIdsToEntityIds   = std::unordered_map<Uuid, Uuid>;
using ShipDbIdsToEntityIds     = std::unordered_map<Uuid, Uuid>;
using AsteroidDbIdsToEntityIds = std::unordered_map<Uuid, Uuid>;

class DatabaseEntityMapper : public utils::CoreObject
{
  public:
  DatabaseEntityMapper();
  ~DatabaseEntityMapper() override = default;

  void setPlayerDbId(const Uuid playerDbId);
  void registerPlayer(const Uuid playerDbId, const Uuid entityId);
  void registerShip(const Uuid shipDbId, const Uuid entityId);
  void registerShipForPlayer(const Uuid playerDbId, const Uuid shipDbId, const Uuid entityId);
  void registerAsteroid(const Uuid asteroidDbId, const Uuid entityId);

  auto tryGetPlayerEntityId() const -> std::optional<Uuid>;
  auto tryGetPlayerShipDbId() const -> std::optional<Uuid>;
  auto tryGetPlayerShipEntityId() const -> std::optional<Uuid>;

  auto tryGetPlayerEntityId(const Uuid playerDbId) const -> std::optional<Uuid>;
  auto tryGetShipEntityId(const Uuid shipDbId) const -> std::optional<Uuid>;
  auto tryGetAsteroidEntityId(const Uuid asteroidDbId) const -> std::optional<Uuid>;

  void clear();

  private:
  mutable std::mutex m_locker{};
  PlayerDbIdsToEntityIds m_playerDbIdsToEntityIds{};
  ShipDbIdsToEntityIds m_shipDbIdsToEntityIds{};
  AsteroidDbIdsToEntityIds m_asteroidDbIdsToEntityIds{};

  std::optional<Uuid> m_playerDbId{};
  std::optional<Uuid> m_playerEntityId{};
  std::optional<Uuid> m_playerShipDbId{};
  std::optional<Uuid> m_playerShipEntityId{};
};

} // namespace bsgo
