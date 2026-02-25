
#include "SystemProcessorAdapter.hh"
#include "AbstractPlayerMessage.hh"

namespace bsgalone::server {

const std::unordered_set<core::MessageType> RELEVANT_MESSAGES = {
  core::MessageType::DOCK,
  core::MessageType::EQUIP,
  core::MessageType::JUMP_CANCELLED,
  core::MessageType::JUMP_REQUESTED,
  core::MessageType::PURCHASE,
  core::MessageType::SHIP_SELECTED,
  core::MessageType::SLOT,
  core::MessageType::TARGET_PICKED,
  core::MessageType::VELOCITY,
};

SystemProcessorAdapter::SystemProcessorAdapter(const bsgo::Uuid systemDbId,
                                               core::IMessageQueueShPtr systemQueue)
  : core::AbstractMessageConsumer("adapter", RELEVANT_MESSAGES)
  , m_systemDbId(systemDbId)
  , m_systemQueue(std::move(systemQueue))
{
  setService("system");
  addModule("processor");

  if (m_systemQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null system queue");
  }
}

void SystemProcessorAdapter::onMessageReceived(const core::IMessage &message)
{
  const auto &playerMessage = message.as<core::AbstractPlayerMessage>();

  if (m_systemDbId != playerMessage.getSystemDbId())
  {
    return;
  }

  m_systemQueue->pushMessage(message.clone());
}

} // namespace bsgalone::server
