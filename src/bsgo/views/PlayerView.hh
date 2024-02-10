
#pragma once

#include "AbstractView.hh"
#include "Faction.hh"
#include "IMessageQueue.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgo {

class PlayerView : public AbstractView
{
  public:
  PlayerView(const Repositories &repositories, IMessageQueue *const outputMessageQueue);
  ~PlayerView() override = default;

  void setPlayerDbId(const std::optional<Uuid> player);

  bool isReady() const noexcept override;

  auto getPlayerDbId() const -> Uuid;
  auto getPlayerFaction() const -> Faction;
  auto getPlayerResources() const -> std::vector<PlayerResource>;
  auto getPlayerWeapons() const -> std::vector<PlayerWeapon>;
  auto getPlayerComputers() const -> std::vector<PlayerComputer>;
  auto getPlayerShips() const -> std::vector<PlayerShip>;

  void trySelectShip(const Uuid shipDbId) const;
  void tryPurchase(const Item &type, const Uuid itemDbId) const;

  void tryLogin(const std::string &name, const std::string &password) const;
  void tryLogout() const;
  void trySignup(const std::string &name, const std::string &password, const Faction &faction) const;

  private:
  Repositories m_repositories{};
  IMessageQueue *const m_outputMessageQueue{};
  std::optional<Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace bsgo
