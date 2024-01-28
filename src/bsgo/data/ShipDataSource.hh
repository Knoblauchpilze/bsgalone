
#pragma once

#include "DataLoadingMode.hh"
#include "DbConnection.hh"
#include "INode.hh"
#include "PlayerDataSource.hh"
#include "PlayerShipRepository.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>
#include <unordered_map>

namespace bsgo {

class Coordinator;

class ShipDataSource : public utils::CoreObject
{
  public:
  ShipDataSource(const Repositories &repositories,
                 const Uuid systemDbId,
                 const PlayerDbIdsToEntityIds &playerDbIdsToEntityIds);
  ShipDataSource(const Repositories &repositories,
                 const Uuid systemDbId,
                 const PlayerDbIdsToEntityIds &playerDbIdsToEntityIds,
                 const Uuid playerDbId);
  ~ShipDataSource() override = default;

  auto getPlayerShipDbId() const -> std::optional<Uuid>;
  auto getPlayerShipEntityId() const -> std::optional<Uuid>;

  void initialize(Coordinator &coordinator) const;

  private:
  DataLoadingMode m_dataLoadingMode{};
  Uuid m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
  PlayerDbIdsToEntityIds m_playerDbIdsToEntityIds{};
  Repositories m_repositories{};

  mutable std::optional<Uuid> m_playerShipDbId{};
  mutable std::optional<Uuid> m_playerShipEntityId{};

  void registerShip(Coordinator &coordinator, const Uuid &ship) const;

  void registerShipOwner(Coordinator &coordinator,
                         const Uuid &shipEntity,
                         const PlayerShip &shipData) const;
  void registerPlayerDataIfNeeded(const Uuid shipEntity, const PlayerShip &shipData) const;

  void registerShipWeapons(Coordinator &coordinator, const Uuid &ship, const Uuid &shipEntity) const;
  void registerShipComputers(Coordinator &coordinator,
                             const Uuid &ship,
                             const Uuid &shipEntity) const;

  auto generateBehaviorTree(const Uuid &entity, const Eigen::Vector3f &center) const -> INodePtr;
};

} // namespace bsgo
