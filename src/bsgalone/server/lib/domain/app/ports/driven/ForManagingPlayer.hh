
#pragma once

#include "Player.hh"
#include <memory>

namespace bsgalone::server {

class ForManagingPlayer
{
  public:
  ForManagingPlayer()          = default;
  virtual ~ForManagingPlayer() = default;

  virtual auto findOneById(const core::Uuid playerDbId) const -> Player       = 0;
  virtual auto findOneByAccount(const core::Uuid accountDbId) const -> Player = 0;
  virtual auto save(Player player) -> Player                                  = 0;
};

using ForManagingPlayerShPtr = std::shared_ptr<ForManagingPlayer>;

} // namespace bsgalone::server
