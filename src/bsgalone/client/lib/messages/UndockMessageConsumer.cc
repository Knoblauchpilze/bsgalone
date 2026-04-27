
#include "UndockMessageConsumer.hh"
#include "UndockEvent.hh"
#include "UndockMessage.hh"

namespace bsgalone::client {

UndockMessageConsumer::UndockMessageConsumer(IDataStoreShPtr store, IUiEventQueueShPtr queue)
  : m_store(std::move(store))
  , m_queue(std::move(queue))
{
  if (m_store == nullptr)
  {
    throw std::invalid_argument("Expected non null data store");
  }
  if (m_queue == nullptr)
  {
    throw std::invalid_argument("Expected non null event queue");
  }
}

bool UndockMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::UNDOCK;
}

void UndockMessageConsumer::onEventReceived(const core::IMessage &event)
{
  const auto &undock = event.as<core::UndockMessage>();

  if (!m_store->isLoggedIn() || m_store->getPlayerDbId() != undock.getPlayerDbId())
  {
    return;
  }

  m_queue->pushEvent(std::make_unique<UndockEvent>());
}

} // namespace bsgalone::client
