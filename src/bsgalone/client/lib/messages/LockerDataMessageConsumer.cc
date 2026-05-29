
#include "LockerDataMessageConsumer.hh"
#include "LockerDataMessage.hh"
#include "LockerReadyEvent.hh"

namespace bsgalone::client {

LockerDataMessageConsumer::LockerDataMessageConsumer(IDataStoreShPtr store, IUiEventQueueShPtr queue)
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

bool LockerDataMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::LOCKER_DATA;
}

void LockerDataMessageConsumer::onEventReceived(const core::IMessage &event)
{
  const auto &hangar = event.as<core::LockerDataMessage>();

  if (!m_store->isLoggedIn() || m_store->getPlayerDbId() != hangar.getPlayerDbId())
  {
    return;
  }

  m_queue->pushEvent(std::make_unique<LockerReadyEvent>());
}

} // namespace bsgalone::client
