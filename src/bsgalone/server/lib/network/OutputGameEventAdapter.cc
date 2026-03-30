
#include "OutputGameEventAdapter.hh"
#include "LoginMessage.hh"
#include "PlayerLoginEvent.hh"

namespace bsgalone::server {

OutputGameEventAdapter::OutputGameEventAdapter(core::IMessageQueueShPtr outputQueue)
  : IGameEventListener()
  , m_outputQueue(std::move(outputQueue))
{
  if (m_outputQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null output queue");
  }
}

bool OutputGameEventAdapter::isEventRelevant(const core::GameEventType &type) const
{
  return type == core::GameEventType::PLAYER_LOGIN;
}

namespace {
void publishLoginMessage(core::IMessageQueue &queue, const core::PlayerLoginEvent &event)
{
  auto message = std::make_unique<core::LoginMessage>(event.getClientId());
  if (event.successfulLogin())
  {
    message->setPlayerDbId(event.tryGetPlayerDbId().value());
    message->setRole(event.tryGetRole().value());
    message->setSystemDbId(event.tryGetSystemDbId().value());
  }

  queue.pushEvent(std::move(message));
}
} // namespace

void OutputGameEventAdapter::onEventReceived(const core::IGameEvent &event)
{
  switch (event.type())
  {
    case core::GameEventType::PLAYER_LOGIN:
      publishLoginMessage(*m_outputQueue, event.as<core::PlayerLoginEvent>());
      break;
    default:
      throw std::invalid_argument("Unsupported game event type " + str(event.type()));
  }
}

} // namespace bsgalone::server
