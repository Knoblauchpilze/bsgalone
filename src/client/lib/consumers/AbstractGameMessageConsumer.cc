
#include "AbstractGameMessageConsumer.hh"
#include "AsteroidDataSource.hh"
#include "AsteroidListMessage.hh"

namespace pge {

AbstractGameMessageConsumer::AbstractGameMessageConsumer(const bool active,
                                                         bsgo::AbstractMessageConsumerPtr consumer)
  : bsgo::AbstractMessageConsumer("abstract", consumer->relevantMessageTypes())
  , m_active(active)
  , m_consumer(std::move(consumer))
{}

void AbstractGameMessageConsumer::activate(const bool active)
{
  m_active = active;
}

void AbstractGameMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  if (m_active)
  {
    m_consumer->onMessageReceived(message);
  }
}

} // namespace pge
