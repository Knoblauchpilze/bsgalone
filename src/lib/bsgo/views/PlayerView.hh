
#pragma once

#include "AbstractView.hh"
#include "Faction.hh"
#include <memory>

namespace bsgo {

class PlayerView : public AbstractView
{
  public:
  PlayerView(const CoordinatorShPtr &coordinator,
             const Repositories &repositories,
             IMessageQueue *const messageQueue);
  ~PlayerView() override = default;

  void setPlayerDbId(const Uuid &player);
  void setPlayerShipDbId(const Uuid &ship);

  bool isReady() const noexcept override;

  auto getPlayerDbId() const -> Uuid;
  auto getPlayerFaction() const -> Faction;
  auto getPlayerResources() const -> std::vector<PlayerResource>;
  auto getPlayerWeapons() const -> std::vector<PlayerWeapon>;
  auto getPlayerComputers() const -> std::vector<PlayerComputer>;
  auto getPlayerShips() const -> std::vector<PlayerShip>;

  void trySelectShip(const Uuid &shipDbId) const;
  void tryPurchase(const Item &type, const Uuid &itemDbId) const;

  void tryLogin(const std::string &name, const std::string &password) const;
  void trySignup(const std::string &name, const std::string &password, const Faction &faction) const;

  private:
  std::optional<Uuid> m_playerDbId{};
  std::optional<Uuid> m_playerShipDbId{};

  void checkPlayerDbIdExists() const;
  void checkPlayerShipDbIdExists() const;
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace bsgo
