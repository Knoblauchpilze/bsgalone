
#pragma once

#include "DbConnection.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>
#include <unordered_map>

namespace bsgo {

class Coordinator;

using PlayerDbIdsToEntityIds = std::unordered_map<Uuid, Uuid>;

class PlayerDataSource : public utils::CoreObject
{
  public:
  PlayerDataSource(const Repositories &repositories,
                   const Uuid systemDbId,
                   const std::optional<Uuid> &playerDbId);
  ~PlayerDataSource() override = default;

  auto getPlayerEntityId() const -> std::optional<Uuid>;

  auto initialize(Coordinator &coordinator) const -> PlayerDbIdsToEntityIds;

  private:
  Uuid m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
  Repositories m_repositories{};

  mutable PlayerDbIdsToEntityIds m_playerDbIdsToEntityIds{};

  void registerPlayer(Coordinator &coordinator, const Uuid playerDbId) const;
  void registerResources(Coordinator &coordinator,
                         const Uuid playerEntityId,
                         const Uuid playerDbId) const;
};

} // namespace bsgo
