
#pragma once

#include "AbstractMessageListener.hh"
#include "DockMessage.hh"
#include "JumpMessage.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <optional>

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

  void handleJumpMessage(const bsgo::JumpMessage &message);
  void handleDockMessage(const bsgo::DockMessage &message);
};

} // namespace pge
