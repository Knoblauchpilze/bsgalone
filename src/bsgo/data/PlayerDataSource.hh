
#pragma once

#include "DbConnection.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class PlayerDataSource : public utils::CoreObject
{
  public:
  PlayerDataSource(const Repositories &repositories, const Uuid &playerDbId);
  ~PlayerDataSource() override = default;

  auto initialize(Coordinator &coordinator) const -> Uuid;

  private:
  Uuid m_playerDbId{};
  Repositories m_repositories{};
};

} // namespace bsgo
