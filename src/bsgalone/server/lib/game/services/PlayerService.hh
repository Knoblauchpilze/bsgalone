
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgalone::server {

class PlayerService : public AbstractService
{
  public:
  PlayerService(const core::Repositories &repositories);
  ~PlayerService() override = default;

  bool tryJoinShip(const core::Uuid playerDbId, const core::Uuid shipDbId) const;

  private:
  void makePlayerJoinShip(const core::Uuid playerDbId, const core::Uuid shipDbId) const;
};

using PlayerServicePtr = std::unique_ptr<PlayerService>;

} // namespace bsgalone::server
