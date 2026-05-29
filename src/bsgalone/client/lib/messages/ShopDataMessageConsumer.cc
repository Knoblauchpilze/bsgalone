
#include "ShopDataMessageConsumer.hh"
#include "ShopDataMessage.hh"
#include "ShopReadyEvent.hh"

namespace bsgalone::client {

ShopDataMessageConsumer::ShopDataMessageConsumer(IDataStoreShPtr store, IUiEventQueueShPtr queue)
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

bool ShopDataMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::SHOP_DATA;
}

void ShopDataMessageConsumer::onEventReceived(const core::IMessage &event)
{
  const auto &hangar = event.as<core::ShopDataMessage>();

  if (!m_store->isLoggedIn() || m_store->getPlayerDbId() != hangar.getPlayerDbId())
  {
    return;
  }

  m_queue->pushEvent(std::make_unique<ShopReadyEvent>());
}

} // namespace bsgalone::client
