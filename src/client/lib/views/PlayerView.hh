
#pragma once

#include "AbstractView.hh"
#include "Faction.hh"
#include "IMessageQueue.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace pge {

class PlayerView : public AbstractView
{
  public:
  PlayerView(const bsgo::Repositories &repositories, bsgo::IMessageQueue *const outputMessageQueue);
  ~PlayerView() override = default;

  void setPlayerDbId(const bsgo::Uuid player);

  bool isReady() const noexcept override;

  auto getPlayerDbId() const -> bsgo::Uuid;
  auto getPlayerFaction() const -> bsgo::Faction;
  auto getPlayerResources() const -> std::vector<bsgo::PlayerResource>;
  auto getPlayerWeapons() const -> std::vector<bsgo::PlayerWeapon>;
  auto getPlayerComputers() const -> std::vector<bsgo::PlayerComputer>;
  auto getPlayerShips() const -> std::vector<bsgo::PlayerShip>;

  void trySelectShip(const bsgo::Uuid shipDbId) const;
  void tryPurchase(const bsgo::Item &type, const bsgo::Uuid itemDbId) const;

  void tryLogin(const std::string &name, const std::string &password) const;
  void tryLogout() const;
  void trySignup(const std::string &name,
                 const std::string &password,
                 const bsgo::Faction &faction) const;

  private:
  bsgo::Repositories m_repositories{};
  bsgo::IMessageQueue *const m_outputMessageQueue{};
  std::optional<bsgo::Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace pge
