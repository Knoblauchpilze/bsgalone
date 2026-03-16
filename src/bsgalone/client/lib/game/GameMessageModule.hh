
#pragma once

#include "AbstractMessageListener.hh"
#include "CoreObject.hh"
#include "Uuid.hh"
#include <optional>

#include "DatabaseEntityMapper.hh"
#include "DockMessage.hh"
#include "EntityRemovedMessage.hh"
#include "HangarMessage.hh"
#include "JumpMessage.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "PlayerLoginDataMessage.hh"
#include "SystemListMessage.hh"

namespace bsgalone::client {

class Game;

class GameMessageModule : public core::AbstractMessageListener, public ::core::CoreObject
{
  public:
  GameMessageModule(Game &game, const core::DatabaseEntityMapper &entityMapper);
  ~GameMessageModule() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  Game &m_game;
  const core::DatabaseEntityMapper &m_entityMapper;

  void handleDockMessage(const core::DockMessage &message);
  void handleHangarMessage(const core::HangarMessage &message);
  void handleJumpMessage(const core::JumpMessage &message);
  void handleLoginMessage(const core::LoginMessage &message);
  void handleLogoutMessage(const core::LogoutMessage &message);
  void handleEntityRemovedMessage(const core::EntityRemovedMessage &message);
  void handleLoadingStartedMessage(const core::LoadingStartedMessage &message) const;
  void handleLoadingFinishedMessage(const core::LoadingFinishedMessage &message) const;
  void handlePlayerLoginDataMessage(const core::PlayerLoginDataMessage &message);
  void handleSystemListMessage(const core::SystemListMessage &message) const;
};

} // namespace bsgalone::client
