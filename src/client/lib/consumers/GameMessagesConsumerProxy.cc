
#include "GameMessagesConsumerProxy.hh"

namespace pge {

GameMessagesConsumerProxy::GameMessagesConsumerProxy(AbstractMessageConsumer &consumer)
  : bsgo::AbstractMessageConsumer("proxy", consumer.relevantMessageTypes())
  , m_consumer(consumer)
{
  addModule("game");
}

void GameMessagesConsumerProxy::onMessageReceived(const bsgo::IMessage &message)
{
  m_consumer.onMessageReceived(message);
}

} // namespace pge
