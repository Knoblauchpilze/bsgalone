
#pragma once

#include "AbstractMessageConsumer.hh"

namespace pge {

class GameMessageConsumerProxy : public bsgo::AbstractMessageConsumer
{
  public:
  GameMessageConsumerProxy(AbstractMessageConsumer &consumer);
  ~GameMessageConsumerProxy() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::AbstractMessageConsumer &m_consumer;
};

} // namespace pge
