
#include "SystemDataMessageConsumer.hh"
#include "GameReadyEvent.hh"
#include "SystemDataMessage.hh"

namespace bsgalone::client {

SystemDataMessageConsumer::SystemDataMessageConsumer(IGameShPtr game, IUiEventQueueShPtr queue)
  : m_game(std::move(game))
  , m_queue(std::move(queue))
{
  if (m_game == nullptr)
  {
    throw std::invalid_argument("Expected non null game");
  }
  if (m_queue == nullptr)
  {
    throw std::invalid_argument("Expected non null event queue");
  }
}

bool SystemDataMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::SYSTEM_DATA;
}

void SystemDataMessageConsumer::onEventReceived(const core::IMessage &event)
{
  const auto &serverData = event.as<core::SystemDataMessage>();
  SystemData data{
    .name        = serverData.getSystemName(),
    .currentTick = serverData.getCurrentTick(),
    .step        = serverData.getTimeStep(),
    .asteroids   = serverData.getAsteroids(),
  };

  m_game->onSystemDataReceived(data);

  m_queue->pushEvent(std::make_unique<GameReadyEvent>(data.name));
}

} // namespace bsgalone::client
