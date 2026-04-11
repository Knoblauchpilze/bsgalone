
#include "LogoutMessageConsumer.hh"
#include "LogoutEvent.hh"
#include "LogoutMessage.hh"

namespace bsgalone::client {

LogoutMessageConsumer::LogoutMessageConsumer(IDataStoreShPtr store, IUiEventQueueShPtr queue)
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

bool LogoutMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::LOGOUT;
}

void LogoutMessageConsumer::onEventReceived(const core::IMessage &event)
{
  const auto &login = event.as<core::LogoutMessage>();

  m_store->onPlayerLoggedOut(login.getPlayerDbId());

  if (!m_store->isLoggedIn())
  {
    m_queue->pushEvent(std::make_unique<LogoutEvent>());
  }
}

} // namespace bsgalone::client
