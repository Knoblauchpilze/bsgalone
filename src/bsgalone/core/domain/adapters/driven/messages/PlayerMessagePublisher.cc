
#include "PlayerMessagePublisher.hh"
#include "SystemListMessage.hh"

namespace bsgalone::core {

PlayerMessagePublisher::PlayerMessagePublisher(IMessageQueueShPtr outputMessageQueue)
  : m_outputMessageQueue(std::move(outputMessageQueue))
{
  if (m_outputMessageQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null output message queue");
  }
}

void PlayerMessagePublisher::publishSystemList(const Uuid playerDbId,
                                               const std::vector<System> &systems)
{
  if (systems.empty())
  {
    return;
  }

  auto out = std::make_unique<SystemListMessage>(playerDbId, systems);

  m_outputMessageQueue->pushEvent(std::move(out));
}

} // namespace bsgalone::core
