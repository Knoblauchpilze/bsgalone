
#include "SystemDataMessageConsumer.hh"
#include "SystemDataMessage.hh"

namespace bsgalone::client {

SystemDataMessageConsumer::SystemDataMessageConsumer(IDataStoreShPtr store)
  : m_store(std::move(store))
{
  if (m_store == nullptr)
  {
    throw std::invalid_argument("Expected non null data store");
  }
}

bool SystemDataMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::SYSTEM_DATA;
}

void SystemDataMessageConsumer::onEventReceived(const core::IMessage &event)
{
  // TODO: Should handle message
  /*const auto &data = */ event.as<core::SystemDataMessage>();
}

} // namespace bsgalone::client
