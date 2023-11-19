
#pragma once

#include "IView.hh"
#include <memory>

namespace bsgo {

class PlayerView : public IView
{
  public:
  PlayerView(const CoordinatorShPtr &coordinator, const Repositories &repositories);
  ~PlayerView() override = default;

  void setPlayerDbId(const Uuid &player);
  void setPlayerShipDbId(const Uuid &ship);

  bool isReady() const noexcept override;

  auto getPlayerResources() const -> std::vector<PlayerResource>;
  auto getPlayerWeapons() const -> std::vector<PlayerWeapon>;
  auto getPlayerComputers() const -> std::vector<PlayerComputer>;
  auto getPlayerShips() const -> std::vector<PlayerShip>;

  private:
  std::optional<Uuid> m_playerDbId{};
  std::optional<Uuid> m_playerShipDbId{};

  void checkPlayerDbIdExists() const;
  void checkPlayerShipDbIdExists() const;
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace bsgo
