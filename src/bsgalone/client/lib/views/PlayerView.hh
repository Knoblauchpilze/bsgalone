
#pragma once

#include "AbstractView.hh"
#include "Faction.hh"
#include "GameRole.hh"
#include "GameSession.hh"
#include "HangarMessage.hh"
#include "IMessageQueue.hh"
#include "PlayerComputerListMessage.hh"
#include "PlayerResourceListMessage.hh"
#include "PlayerShipListMessage.hh"
#include "PlayerWeaponListMessage.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgalone::client {

class PlayerView : public AbstractView
{
  public:
  PlayerView(GameSessionShPtr gameSession, core::IMessageQueue *const outputMessageQueue);
  ~PlayerView() override = default;

  bool isReady() const noexcept override;
  void reset() override;

  auto gameSession() const -> const GameSession &;

  auto getPlayerFaction() const -> core::Faction;
  auto getPlayerResources() const -> std::vector<core::PlayerResourceData>;
  auto getPlayerWeapons() const -> std::vector<core::PlayerWeaponData>;
  auto getPlayerComputers() const -> std::vector<core::PlayerComputerData>;
  auto getPlayerShips() const -> std::vector<core::PlayerShipData>;

  void trySelectShip(const core::Uuid shipDbId) const;
  void tryPurchase(const core::Item &type, const core::Uuid itemDbId) const;

  void tryLogin(const std::string &name,
                const std::string &password,
                const core::GameRole role) const;
  void tryLogout() const;
  void trySignup(const std::string &name,
                 const std::string &password,
                 const core::Faction &faction) const;

  void tryJoin(const core::Uuid playerDbId, const core::Uuid shipDbId) const;

  protected:
  void handleMessageInternal(const core::IMessage &message) override;

  private:
  GameSessionShPtr m_gameSession{};
  core::IMessageQueue *const m_outputMessageQueue{};

  std::vector<core::PlayerResourceData> m_playerResources{};
  std::vector<core::PlayerShipData> m_playerShips{};
  std::vector<core::PlayerComputerData> m_playerComputers{};
  std::vector<core::PlayerWeaponData> m_playerWeapons{};

  void handleHangarMessage(const core::HangarMessage &message);
  void handlePlayerComputersMessage(const core::PlayerComputerListMessage &message);
  void handlePlayerResourcesMessage(const core::PlayerResourceListMessage &message);
  void handlePlayerShipsMessage(const core::PlayerShipListMessage &message);
  void handlePlayerWeaponsMessage(const core::PlayerWeaponListMessage &message);
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace bsgalone::client
