
#pragma once

#include "AbstractMessageListener.hh"
#include "CoreObject.hh"
#include "Uuid.hh"
#include <optional>

#include "ConnectionMessage.hh"
#include "DatabaseEntityMapper.hh"
#include "DockMessage.hh"
#include "EntityRemovedMessage.hh"
#include "HangarMessage.hh"
#include "JumpMessage.hh"
#include "LoadingFinishedMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "SignupMessage.hh"

namespace pge {

class Game;

class GameMessageModule : public bsgo::AbstractMessageListener, public core::CoreObject
{
  public:
  GameMessageModule(Game &game, const bsgo::DatabaseEntityMapper &entityMapper);
  ~GameMessageModule() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  Game &m_game;
  const bsgo::DatabaseEntityMapper &m_entityMapper;

  // TODO: We can include in the GameDataModule (see DataSource) the desired
  // next screen. This consumer will call a new onDataLoaded method and in the
  // Game we can interrogate this module and set the screen accordingly.

  void handleConnectionMessage(const bsgo::ConnectionMessage &message);
  void handleDockMessage(const bsgo::DockMessage &message);
  void handleHangarMessage(const bsgo::HangarMessage &message);
  void handleJumpMessage(const bsgo::JumpMessage &message);
  void handleLoginMessage(const bsgo::LoginMessage &message);
  void handleLogoutMessage(const bsgo::LogoutMessage &message);
  void handleSignupMessage(const bsgo::SignupMessage &message);
  void handleEntityRemovedMessage(const bsgo::EntityRemovedMessage &message);
  void handleLoadingFinishedMessage(const bsgo::LoadingFinishedMessage &message) const;
};

} // namespace pge
