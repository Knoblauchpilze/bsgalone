
#pragma once

#include "IView.hh"
#include <memory>

namespace bsgo {

class PlayerView : public IView
{
  public:
  PlayerView(const Uuid &playerId,
             const Uuid &playerShipId,
             const CoordinatorShPtr &coordinator,
             const Repositories &repositories);
  ~PlayerView() override = default;

  auto getPlayerResources() const -> std::vector<PlayerResource>;
  auto getPlayerWeapons() const -> std::vector<PlayerWeapon>;
  auto getPlayerComputers() const -> std::vector<PlayerComputer>;

  private:
  Uuid m_playerId;
  Uuid m_playerShipId;
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace bsgo
