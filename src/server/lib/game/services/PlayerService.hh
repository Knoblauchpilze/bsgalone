
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class PlayerService : public AbstractService
{
  public:
  PlayerService(const Repositories &repositories);
  ~PlayerService() override = default;

  bool tryJoinShip(const Uuid playerDbId, const Uuid shipDbId) const;

  private:
  void makePlayerJoinShip(const Uuid playerDbId, const Uuid shipDbId) const;
};

using PlayerServicePtr = std::unique_ptr<PlayerService>;

} // namespace bsgo
