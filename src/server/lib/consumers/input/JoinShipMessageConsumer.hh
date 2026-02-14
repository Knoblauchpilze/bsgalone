
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "JoinShipMessage.hh"
#include "PlayerService.hh"

namespace bsgo {

class JoinShipMessageConsumer : public AbstractMessageConsumer
{
  public:
  JoinShipMessageConsumer(PlayerServicePtr playerService, IMessageQueue *const outputMessageQueue);
  ~JoinShipMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  PlayerServicePtr m_playerService{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleJoinShip(const JoinShipMessage &message) const;
};

} // namespace bsgo
