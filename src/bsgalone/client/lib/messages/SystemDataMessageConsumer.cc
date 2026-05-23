
#include "SystemDataMessageConsumer.hh"
#include "SystemDataMessage.hh"

#include <iostream>

namespace bsgalone::client {

SystemDataMessageConsumer::SystemDataMessageConsumer(IGameShPtr game)
  : m_game(std::move(game))
{
  if (m_game == nullptr)
  {
    throw std::invalid_argument("Expected non null game");
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
  };

  m_game->onSystemDataReceived(data);
}

} // namespace bsgalone::client
