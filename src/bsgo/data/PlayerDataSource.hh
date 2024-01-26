
#pragma once

#include "DbConnection.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class PlayerDataSource : public utils::CoreObject
{
  public:
  PlayerDataSource(const Repositories &repositories,
                   const Uuid systemDbId,
                   const std::optional<Uuid> &playerDbId);
  ~PlayerDataSource() override = default;

  auto initialize(Coordinator &coordinator) const -> std::optional<Uuid>;

  private:
  Uuid m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
  mutable std::optional<Uuid> m_playerEntityId{};
  Repositories m_repositories{};

  void registerPlayer(Coordinator &coordinator, const Uuid playerId) const;
};

} // namespace bsgo
