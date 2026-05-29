
#include "HangarDataMessageConsumer.hh"
#include "HangarDataMessage.hh"
#include "HangarReadyEvent.hh"

namespace bsgalone::client {

HangarDataMessageConsumer::HangarDataMessageConsumer(IDataStoreShPtr store, IUiEventQueueShPtr queue)
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

bool HangarDataMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::HANGAR_DATA;
}

void HangarDataMessageConsumer::onEventReceived(const core::IMessage &event)
{
  const auto &hangar = event.as<core::HangarDataMessage>();

  if (!m_store->isLoggedIn() || m_store->getPlayerDbId() != hangar.getPlayerDbId())
  {
    return;
  }

  m_queue->pushEvent(std::make_unique<HangarReadyEvent>());
}

} // namespace bsgalone::client
