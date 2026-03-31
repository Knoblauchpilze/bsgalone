
#include "LoginMessageConsumer.hh"

namespace bsgalone::client {

// TODO: Add tests for this class
LoginMessageConsumer::LoginMessageConsumer(IDataStoreShPtr store)
  : m_store(std::move(store))
{}

bool LoginMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::LOGIN;
}

void LoginMessageConsumer::onEventReceived(const core::IMessage & /*event*/) {}

} // namespace bsgalone::client
