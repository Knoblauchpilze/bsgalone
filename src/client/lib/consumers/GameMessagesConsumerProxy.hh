
#pragma once

#include "AbstractMessageConsumer.hh"

namespace pge {

class GameMessagesConsumerProxy : public bsgo::AbstractMessageConsumer
{
  public:
  GameMessagesConsumerProxy(AbstractMessageConsumer &consumer);
  ~GameMessagesConsumerProxy() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::AbstractMessageConsumer &m_consumer;
};

} // namespace pge
