
#include "OutputUiCommandAdapter.hh"
#include "LoginCommand.hh"
#include "LoginRequest.hh"
#include "SignupCommand.hh"
#include "SignupRequest.hh"

namespace bsgalone::client {

OutputUiCommandAdapter::OutputUiCommandAdapter(core::IMessageQueueShPtr outputQueue)
  : IUiCommandListener()
  , m_outputQueue(std::move(outputQueue))
{
  if (m_outputQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null output queue");
  }
}

bool OutputUiCommandAdapter::isEventRelevant(const UiCommandType &type) const
{
  return type == UiCommandType::LOGIN_REQUESTED || type == UiCommandType::SIGNUP_REQUESTED;
}

namespace {
void publishLoginRequest(core::IMessageQueue &queue, const LoginCommand &command)
{
  queue.pushEvent(std::make_unique<core::LoginRequest>(command.getUsername(),
                                                       command.getPassword(),
                                                       command.getRole()));
}

void publishSignupRequest(core::IMessageQueue &queue, const SignupCommand &command)
{
  queue.pushEvent(std::make_unique<core::SignupRequest>(command.getUsername(),
                                                        command.getPassword(),
                                                        command.getFaction()));
}
} // namespace

void OutputUiCommandAdapter::onEventReceived(const IUiCommand &event)
{
  switch (event.type())
  {
    case UiCommandType::LOGIN_REQUESTED:
      publishLoginRequest(*m_outputQueue, event.as<LoginCommand>());
      break;
    case UiCommandType::SIGNUP_REQUESTED:
      publishSignupRequest(*m_outputQueue, event.as<SignupCommand>());
      break;
    default:
      throw std::invalid_argument("Unsupported command type " + str(event.type()));
  }
}

} // namespace bsgalone::client
