
#pragma once

#include "AbstractMessageListener.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <optional>

#include "ConnectionMessage.hh"
#include "DockMessage.hh"
#include "EntityDiedMessage.hh"
#include "HangarMessage.hh"
#include "JumpMessage.hh"
#include "LoginMessage.hh"
#include "SignupMessage.hh"
#include "DatabaseEntityMapper.hh"

namespace pge {

class Game;

class GameMessageModule : public bsgo::AbstractMessageListener, public utils::CoreObject
{
  public:
  GameMessageModule(Game &game, const bsgo::DatabaseEntityMapper& entityMapper);
  ~GameMessageModule() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  Game &m_game;
  const bsgo::DatabaseEntityMapper& m_entityMapper;

  void handleConnectionMessage(const bsgo::ConnectionMessage &message);
  void handleDockMessage(const bsgo::DockMessage &message);
  void handleHangarMessage(const bsgo::HangarMessage &message);
  void handleJumpMessage(const bsgo::JumpMessage &message);
  void handleLoginMessage(const bsgo::LoginMessage &message);
  void handleSignupMessage(const bsgo::SignupMessage &message);
  void handleEntityDiedMessage(const bsgo::EntityDiedMessage& message);
};

} // namespace pge
