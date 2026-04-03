
#include "SignupMessageConsumer.hh"
#include "SignupFailedEvent.hh"
#include "SignupMessage.hh"
#include "SignupSucceededEvent.hh"

namespace bsgalone::client {

SignupMessageConsumer::SignupMessageConsumer(IUiEventQueueShPtr queue)
  : m_queue(std::move(queue))
{
  if (m_queue == nullptr)
  {
    throw std::invalid_argument("Expected non null event queue");
  }
}

bool SignupMessageConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::SIGNUP;
}

void SignupMessageConsumer::onEventReceived(const core::IMessage &event)
{
  const auto &login = event.as<core::SignupMessage>();

  if (!login.successfullySignedUp())
  {
    m_queue->pushEvent(std::make_unique<SignupFailedEvent>());
    return;
  }

  m_queue->pushEvent(std::make_unique<SignupSucceededEvent>());
}

} // namespace bsgalone::client
