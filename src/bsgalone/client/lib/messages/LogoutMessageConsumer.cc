
#include "LogoutMessageConsumer.hh"
#include "LogoutEvent.hh"
#include "LogoutMessage.hh"

namespace bsgalone::client {

LogoutMessageConsumer::LogoutMessageConsumer(IDataStoreShPtr store,
                                             IGameShPtr game,
                                             IUiEventQueueShPtr queue)
  : m_store(std::move(store))
  , m_game(std::move(game))
  , m_queue(std::move(queue))
{
  if (m_store == nullptr)
  {
    throw std::invalid_argument("Expected non null data store");
  }
  if (m_game == nullptr)
  {
    throw std::invalid_argument("Expected non null game");
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

  if (!m_store->isLoggedIn() || m_store->getPlayerDbId() != login.getPlayerDbId())
  {
    return;
  }

  m_store->onPlayerLoggedOut();
  m_game->reset();
  m_queue->pushEvent(std::make_unique<LogoutEvent>());
}

} // namespace bsgalone::client
