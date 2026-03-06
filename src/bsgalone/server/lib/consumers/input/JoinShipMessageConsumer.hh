
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "JoinShipMessage.hh"
#include "PlayerService.hh"

namespace bsgalone::server {

class JoinShipMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  JoinShipMessageConsumer(PlayerServicePtr playerService,
                          core::IMessageQueue *const outputMessageQueue);
  ~JoinShipMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  PlayerServicePtr m_playerService{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleJoinShip(const core::JoinShipMessage &message) const;
};

} // namespace bsgalone::server
