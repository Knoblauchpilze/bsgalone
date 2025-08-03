
#pragma once

#include "AbstractView.hh"
#include "Faction.hh"
#include "GameSession.hh"
#include "IMessageQueue.hh"
#include "PlayerComputerListMessage.hh"
#include "PlayerResourceListMessage.hh"
#include "PlayerShipListMessage.hh"
#include "PlayerWeaponListMessage.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace pge {

class PlayerView : public AbstractView
{
  public:
  PlayerView(const bsgo::Repositories &repositories,
             GameSessionShPtr gameSession,
             bsgo::IMessageQueue *const outputMessageQueue);
  ~PlayerView() override = default;

  bool isReady() const noexcept override;

  void onMessageReceived(const bsgo::IMessage &message) override;

  auto getPlayerDbId() const -> bsgo::Uuid;
  auto getPlayerFaction() const -> bsgo::Faction;
  auto getPlayerResources() const -> std::vector<bsgo::PlayerResourceData>;
  auto getPlayerWeapons() const -> std::vector<bsgo::PlayerWeaponData>;
  auto getPlayerComputers() const -> std::vector<bsgo::PlayerComputerData>;
  auto getPlayerShips() const -> std::vector<bsgo::PlayerShipData>;

  void trySelectShip(const bsgo::Uuid shipDbId) const;
  void tryPurchase(const bsgo::Item &type, const bsgo::Uuid itemDbId) const;

  void tryLogin(const std::string &name, const std::string &password) const;
  void tryLogout() const;
  void trySignup(const std::string &name,
                 const std::string &password,
                 const bsgo::Faction &faction) const;

  private:
  bsgo::Repositories m_repositories{};
  GameSessionShPtr m_gameSession{};
  bsgo::IMessageQueue *const m_outputMessageQueue{};

  std::vector<bsgo::PlayerResourceData> m_playerResources{};
  std::vector<bsgo::PlayerShipData> m_playerShips{};
  std::vector<bsgo::PlayerComputerData> m_playerComputers{};
  std::vector<bsgo::PlayerWeaponData> m_playerWeapons{};

  void handlePlayerComputersMessage(const bsgo::PlayerComputerListMessage &message);
  void handlePlayerResourcesMessage(const bsgo::PlayerResourceListMessage &message);
  void handlePlayerShipsMessage(const bsgo::PlayerShipListMessage &message);
  void handlePlayerWeaponsMessage(const bsgo::PlayerWeaponListMessage &message);
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace pge
