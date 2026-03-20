
#pragma once

#include "Player.hh"
#include <memory>

namespace bsgalone::core {

class ForManagingPlayer
{
  public:
  ForManagingPlayer()          = default;
  virtual ~ForManagingPlayer() = default;

  virtual auto findOneByAccount(const Uuid account) const -> Player = 0;
};

using ForManagingPlayerShPtr = std::shared_ptr<ForManagingPlayer>;

} // namespace bsgalone::core
