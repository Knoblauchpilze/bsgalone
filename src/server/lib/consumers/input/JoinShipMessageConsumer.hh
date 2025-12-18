
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "JoinShipMessage.hh"

namespace bsgo {

class JoinShipMessageConsumer : public AbstractMessageConsumer
{
  public:
  JoinShipMessageConsumer(IMessageQueue *const outputMessageQueue);
  ~JoinShipMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  IMessageQueue *const m_outputMessageQueue{};

  void handleJoinShip(const JoinShipMessage &message) const;
};

} // namespace bsgo
