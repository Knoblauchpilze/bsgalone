
#pragma once

#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <optional>
#include <unordered_map>

namespace bsgo {

using PlayerDbIdsToEntityIds = std::unordered_map<Uuid, Uuid>;
using ShipDbIdsToEntityIds   = std::unordered_map<Uuid, Uuid>;

class DatabaseEntityMapper : public utils::CoreObject
{
  public:
  DatabaseEntityMapper();
  ~DatabaseEntityMapper() override = default;

  void setPlayerDbId(const Uuid playerDbId);
  void registerPlayer(const Uuid playerDbId, const Uuid entityId);
  void registerShip(const Uuid shipDbId, const Uuid entityId);
  void registerShipForPlayer(const Uuid playerDbId, const Uuid shipDbId, const Uuid entityId);

  auto tryGetPlayerEntityId() const -> std::optional<Uuid>;
  auto playerEntityId() const -> Uuid;
  auto playerShipDbId() const -> Uuid;
  auto playerShipEntityId() const -> Uuid;

  auto tryGetPlayerEntityId(const Uuid playerDbId) const -> std::optional<Uuid>;

  void clear();

  private:
  PlayerDbIdsToEntityIds m_playerDbIdsToEntityIds{};
  ShipDbIdsToEntityIds m_shipDbIdsToEntityIds{};

  std::optional<Uuid> m_playerDbId{};
  std::optional<Uuid> m_playerEntityId{};
  std::optional<Uuid> m_playerShipDbId{};
  std::optional<Uuid> m_playerShipEntityId{};
};

} // namespace bsgo
