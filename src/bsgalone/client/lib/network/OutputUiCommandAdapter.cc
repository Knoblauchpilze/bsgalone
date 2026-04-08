
#include "OutputUiCommandAdapter.hh"
#include "LoginCommand.hh"
#include "LoginRequest.hh"
#include "LogoutCommand.hh"
#include "LogoutRequest.hh"
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

namespace {
const std::unordered_set<UiCommandType> RELEVANT_COMMAND_TYPES = {
  UiCommandType::LOGIN_REQUESTED,
  UiCommandType::LOGOUT_REQUESTED,
  UiCommandType::SIGNUP_REQUESTED,
};
}

bool OutputUiCommandAdapter::isEventRelevant(const UiCommandType &type) const
{
  return RELEVANT_COMMAND_TYPES.contains(type);
}

namespace {
void publishLoginRequest(core::IMessageQueue &queue, const LoginCommand &command)
{
  queue.pushEvent(std::make_unique<core::LoginRequest>(command.getUsername(),
                                                       command.getPassword(),
                                                       command.getRole()));
}

void publishLogoutRequest(core::IMessageQueue &queue, const LogoutCommand & /*command*/)
{
  // TODO: Player identifier should be real
  // TODO: This could be achieved by having the server data store in the adapter
  // and the player can be retrieved from it
  queue.pushEvent(std::make_unique<core::LogoutRequest>(core::Uuid{7}));
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
    case UiCommandType::LOGOUT_REQUESTED:
      publishLogoutRequest(*m_outputQueue, event.as<LogoutCommand>());
      break;
    case UiCommandType::SIGNUP_REQUESTED:
      publishSignupRequest(*m_outputQueue, event.as<SignupCommand>());
      break;
    default:
      throw std::invalid_argument("Unsupported command type " + str(event.type()));
  }
}

} // namespace bsgalone::client
