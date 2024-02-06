
#pragma once

#include "DatabaseEntityMapper.hh"
#include "INode.hh"
#include "PlayerShipRepository.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>
#include <unordered_map>

namespace bsgo {

class Coordinator;

class ShipDataSource : public utils::CoreObject
{
  public:
  ShipDataSource(const Repositories &repositories);
  ~ShipDataSource() override = default;

  void initialize(const Uuid systemDbId,
                  Coordinator &coordinator,
                  DatabaseEntityMapper &entityMapper) const;

  void registerShip(Coordinator &coordinator,
                    const Uuid ship,
                    DatabaseEntityMapper &entityMapper) const;

  private:
  std::optional<Uuid> m_playerDbId{};
  Repositories m_repositories{};

  void registerShip(Coordinator &coordinator,
                    const Uuid ship,
                    DatabaseEntityMapper &entityMapper,
                    const bool ignoreDocked) const;
  void registerShipOwner(Coordinator &coordinator,
                         const Uuid shipEntity,
                         const PlayerShip &shipData,
                         DatabaseEntityMapper &entityMapper) const;

  void registerShipWeapons(Coordinator &coordinator, const Uuid ship, const Uuid shipEntity) const;
  void registerShipComputers(Coordinator &coordinator, const Uuid ship, const Uuid shipEntity) const;

  auto generateBehaviorTree(const Uuid entity, const Eigen::Vector3f &center) const -> INodePtr;
};

} // namespace bsgo
