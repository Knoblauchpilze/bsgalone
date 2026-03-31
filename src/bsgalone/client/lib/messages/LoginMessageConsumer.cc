
#include "LoginMessageConsumer.hh"
#include "LoginMessage.hh"

namespace bsgalone::client {

// TODO: Add tests for this class
LoginMessageConsumer::LoginMessageConsumer(IDataStoreShPtr store)
  : m_store(std::move(store))
{
  if (m_store == nullptr)
  {
    throw std::invalid_argument("Expected non null data store");
  }
}

bool LoginMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::LOGIN;
}

void LoginMessageConsumer::onEventReceived(const core::IMessage &event)
{
  const auto &login = event.as<core::LoginMessage>();

  if (!login.successfullyLoggedIn())
  {
    return;
  }

  m_store->onPlayerLoggedIn(login.getPlayerDbId(), login.getSystemDbId(), login.getRole());
}

} // namespace bsgalone::client
