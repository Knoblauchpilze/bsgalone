
#include "InputGameMessageAdapter.hh"
#include "SystemListMessage.hh"
#include "SystemsListReceivedEvent.hh"

namespace bsgalone::client {

const std::unordered_set<core::MessageType> RELEVANT_MESSAGE_TYPES = {
  core::MessageType::SYSTEM_LIST,
};

InputGameMessageAdapter::InputGameMessageAdapter(IUiEventQueueWPtr uiQueue)
  : AbstractMessageListener(RELEVANT_MESSAGE_TYPES)
  , m_uiQueue(std::move(uiQueue))
{}

void InputGameMessageAdapter::onEventReceived(const core::IMessage &event)
{
  switch (event.type())
  {
    case core::MessageType::SYSTEM_LIST:
      tryPushUiEvent(SystemsListReceivedEvent(event.as<core::SystemListMessage>().getSystemsData()));
      break;
    default:
      throw std::invalid_argument("Unsupported event type " + str(event.type()));
  }
}

void InputGameMessageAdapter::tryPushUiEvent(const IUiEvent &event)
{
  auto eventQueue = m_uiQueue.lock();
  if (eventQueue)
  {
    eventQueue->pushEvent(event.clone());
  }
}

} // namespace bsgalone::client
