
#include "OutputUiCommandAdapter.hh"
#include "LoginMessage.hh"

namespace bsgalone::client {

OutputUiCommandAdapter::OutputUiCommandAdapter(core::IMessageQueueShPtr outputQueue)
  : IUiEventListener()
  , m_outputQueue(std::move(outputQueue))
{
  if (m_outputQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null output queue");
  }
}

bool OutputUiCommandAdapter::isEventRelevant(const UiEventType &type) const
{
  return type == UiEventType::LOGIN_REQUESTED;
}

void OutputUiCommandAdapter::onEventReceived(const IUiEvent &event)
{
  switch (event.type())
  {
    case UiEventType::LOGIN_REQUESTED:
      publishLoginMessage(event.as<LoginCommand>());
      break;
    default:
      throw std::invalid_argument("Unsupported event type " + str(event.type()));
  }
}

void OutputUiCommandAdapter::publishLoginMessage(const LoginCommand &command)
{
  auto login = std::make_unique<core::LoginMessage>(command.getRole());
  login->setUserName(command.getUsername());
  login->setPassword(command.getPassword());

  m_outputQueue->pushEvent(std::move(login));
}

} // namespace bsgalone::client
