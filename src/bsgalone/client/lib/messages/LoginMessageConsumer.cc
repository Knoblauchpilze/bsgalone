
#include "LoginMessageConsumer.hh"
#include "LoginFailedEvent.hh"
#include "LoginMessage.hh"
#include "LoginSucceededEvent.hh"

namespace bsgalone::client {

LoginMessageConsumer::LoginMessageConsumer(IDataStoreShPtr store, IUiEventQueueShPtr queue)
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

bool LoginMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::LOGIN;
}

void LoginMessageConsumer::onEventReceived(const core::IMessage &event)
{
  const auto &login = event.as<core::LoginMessage>();

  if (!login.successfullyLoggedIn())
  {
    m_queue->pushEvent(std::make_unique<LoginFailedEvent>());
    return;
  }

  m_store->onPlayerLoggedIn(login.getPlayerDbId(), login.getSystemDbId(), login.getRole());
  m_queue->pushEvent(std::make_unique<LoginSucceededEvent>());
}

} // namespace bsgalone::client
