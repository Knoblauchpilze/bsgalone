
#pragma once

#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "INode.hh"
#include "PlayerShipData.hh"
#include "PlayerShipRepository.hh"
#include "Repositories.hh"
#include <optional>

namespace bsgo {

class Coordinator;

class ShipDataSource : public core::CoreObject
{
  public:
  ShipDataSource();
  ShipDataSource(const Repositories &repositories);
  ~ShipDataSource() override = default;

  void initialize(const Uuid systemDbId,
                  Coordinator &coordinator,
                  DatabaseEntityMapper &entityMapper) const;

  void registerShip(Coordinator &coordinator,
                    const PlayerShipData &data,
                    DatabaseEntityMapper &entityMapper,
                    const bool ignoreIfDocked) const;

  // Thid variant is currently used by the server in two cases:
  //   - when a player leaves the outpost and their ship is created
  //   - when a player jumps to another system and their ship is created
  // We could make it so that those two cases also use the overload above
  // but it's not a priority.
  void registerShip(Coordinator &coordinator,
                    const Uuid ship,
                    DatabaseEntityMapper &entityMapper) const;

  private:
  std::optional<Uuid> m_playerDbId{};
  std::optional<Repositories> m_repositories{};

  void registerShip(Coordinator &coordinator,
                    const Uuid shipDbId,
                    DatabaseEntityMapper &entityMapper,
                    const bool ignoreDocked) const;

  void registerShipOwner(Coordinator &coordinator,
                         const Uuid shipEntity,
                         const PlayerShipData &data,
                         DatabaseEntityMapper &entityMapper) const;
  void registerShipWeapons(Coordinator &coordinator,
                           const PlayerShipData &data,
                           const Uuid shipEntity) const;
  void registerShipComputers(Coordinator &coordinator,
                             const PlayerShipData &data,
                             const Uuid shipEntity) const;

  auto generateBehaviorTree(const Uuid dbId, const int seed, const Eigen::Vector3f &center) const
    -> INodePtr;
};

} // namespace bsgo
