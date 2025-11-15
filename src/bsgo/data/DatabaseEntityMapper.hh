
#pragma once

#include "CoreObject.hh"
#include "Uuid.hh"
#include <mutex>
#include <optional>
#include <unordered_map>

namespace bsgo {

using DbIdsToEntityIds = std::unordered_map<Uuid, Uuid>;

class DatabaseEntityMapper : public core::CoreObject
{
  public:
  DatabaseEntityMapper();
  ~DatabaseEntityMapper() override = default;

  void setPlayerDbId(const Uuid playerDbId);
  bool doesPlayerHaveAnEntity() const;
  void setPlayerShipDbId(const Uuid playerShipDbId);

  void registerPlayer(const Uuid playerDbId, const Uuid entityId);
  void registerShipForPlayer(const Uuid playerDbId, const Uuid shipDbId, const Uuid entityId);
  void registerAsteroid(const Uuid asteroidDbId, const Uuid entityId);
  void registerOutpost(const Uuid outpostDbId, const Uuid entityId);

  void removeEntityForPlayer(const Uuid playerDbId);
  bool tryRemoveEntityForShip(const Uuid shipDbId);
  void removeEntityForShip(const Uuid shipDbId);
  void removeEntityForAsteroid(const Uuid asteroidDbId);

  auto tryGetPlayerDbId() const -> std::optional<Uuid>;
  auto tryGetPlayerEntityId() const -> std::optional<Uuid>;
  auto tryGetPlayerShipDbId() const -> std::optional<Uuid>;
  auto tryGetPlayerShipEntityId() const -> std::optional<Uuid>;

  auto tryGetPlayerEntityId(const Uuid playerDbId) const -> std::optional<Uuid>;
  auto tryGetShipEntityId(const Uuid shipDbId) const -> std::optional<Uuid>;
  auto tryGetAsteroidEntityId(const Uuid asteroidDbId) const -> std::optional<Uuid>;
  auto tryGetOutpostEntityId(const Uuid outpostDbId) const -> std::optional<Uuid>;

  void clearEntities();
  void clearAll();

  private:
  mutable std::mutex m_locker{};
  DbIdsToEntityIds m_playerDbIdsToEntityIds{};
  DbIdsToEntityIds m_shipDbIdsToEntityIds{};
  DbIdsToEntityIds m_asteroidDbIdsToEntityIds{};
  DbIdsToEntityIds m_outpostDbIdsToEntityIds{};

  std::optional<Uuid> m_playerDbId{};
  std::optional<Uuid> m_playerEntityId{};
  std::optional<Uuid> m_playerShipDbId{};
  std::optional<Uuid> m_playerShipEntityId{};

  void registerShip(const Uuid shipDbId, const Uuid entityId);
};

} // namespace bsgo
