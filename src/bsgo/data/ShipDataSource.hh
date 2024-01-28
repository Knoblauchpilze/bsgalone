
#pragma once

#include "DataLoadingMode.hh"
#include "DbConnection.hh"
#include "INode.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class ShipDataSource : public utils::CoreObject
{
  public:
  ShipDataSource(const Repositories &repositories, const Uuid systemDbId);
  ShipDataSource(const Repositories &repositories, const Uuid playerDbId, const Uuid playerEntityId);
  ~ShipDataSource() override = default;

  auto getPlayerShipDbId() const -> std::optional<Uuid>;
  auto getPlayerShipEntityId() const -> std::optional<Uuid>;

  void initialize(Coordinator &coordinator) const;

  private:
  Uuid m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
  std::optional<Uuid> m_playerEntityId{};
  Repositories m_repositories{};

  mutable std::optional<Uuid> m_playerShipDbId{};
  mutable std::optional<Uuid> m_playerShipEntityId{};

  ShipDataSource(const Repositories &repositories);

  void registerShip(Coordinator &coordinator, const Uuid &ship) const;

  void registerShipWeapons(Coordinator &coordinator, const Uuid &ship, const Uuid &shipEntity) const;
  void registerShipComputers(Coordinator &coordinator,
                             const Uuid &ship,
                             const Uuid &shipEntity) const;

  auto generateBehaviorTree(const Uuid &entity, const Eigen::Vector3f &center) const -> INodePtr;
};

} // namespace bsgo
