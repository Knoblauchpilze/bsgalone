
#include "GameEventPublisher.hh"

namespace bsgalone::core {

GameEventPublisher::GameEventPublisher(server::IGameEventQueueShPtr queue)
  : m_queue(std::move(queue))
{
  if (m_queue == nullptr)
  {
    throw std::invalid_argument("Expected non null event queue");
  }
}

void GameEventPublisher::publishEvent(IGameEventPtr event)
{
  m_queue->pushEvent(std::move(event));
}

} // namespace bsgalone::core
