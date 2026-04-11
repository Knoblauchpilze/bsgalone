
#include "LogoutCommand.hh"

namespace bsgalone::client {

LogoutCommand::LogoutCommand()
  : IUiCommand(UiCommandType::LOGOUT_REQUESTED)
{}

auto LogoutCommand::clone() const -> IUiCommandPtr
{
  return std::make_unique<LogoutCommand>();
}

} // namespace bsgalone::client
