
#include "GameMessageConsumerProxy.hh"

namespace pge {

GameMessageConsumerProxy::GameMessageConsumerProxy(AbstractMessageConsumer &consumer)
  : bsgo::AbstractMessageConsumer("proxy", consumer.relevantMessageTypes())
  , m_consumer(consumer)
{
  addModule("game");
}

void GameMessageConsumerProxy::onMessageReceived(const bsgo::IMessage &message)
{
  m_consumer.onMessageReceived(message);
}

} // namespace pge
