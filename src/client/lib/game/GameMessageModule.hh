
#pragma once

#include "AbstractMessageListener.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <optional>

#include "DockMessage.hh"
#include "HangarMessage.hh"
#include "JumpMessage.hh"
#include "LoginMessage.hh"
#include "SignupMessage.hh"

namespace pge {

class Game;

class GameMessageModule : public bsgo::AbstractMessageListener, public utils::CoreObject
{
  public:
  GameMessageModule(Game *game);
  ~GameMessageModule() override = default;

  void setPlayerShipDbId(const bsgo::Uuid &shipDbId);

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  Game *m_game{};
  std::optional<bsgo::Uuid> m_playerShipDbId{};

  void checkIfPlayerShipDbIdExists();

  void handleDockMessage(const bsgo::DockMessage &message);
  void handleHangarMessage(const bsgo::HangarMessage &message);
  void handleJumpMessage(const bsgo::JumpMessage &message);
  void handleLoginMessage(const bsgo::LoginMessage &message);
  void handleSignupMessage(const bsgo::SignupMessage &message);
};

} // namespace pge
