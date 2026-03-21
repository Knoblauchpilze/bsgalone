
#pragma once

#include "Player.hh"
#include <memory>

namespace bsgalone::core {

class ForManagingPlayer
{
  public:
  ForManagingPlayer()          = default;
  virtual ~ForManagingPlayer() = default;

  virtual auto findOneById(const Uuid playerDbId) const -> Player       = 0;
  virtual auto findOneByAccount(const Uuid accountDbId) const -> Player = 0;
};

using ForManagingPlayerShPtr = std::shared_ptr<ForManagingPlayer>;

} // namespace bsgalone::core
