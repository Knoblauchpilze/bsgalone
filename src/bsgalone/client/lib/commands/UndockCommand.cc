
#include "UndockCommand.hh"

namespace bsgalone::client {

UndockCommand::UndockCommand()
  : IUiCommand(UiCommandType::UNDOCK_REQUESTED)
{}

auto UndockCommand::clone() const -> IUiCommandPtr
{
  return std::make_unique<UndockCommand>();
}

} // namespace bsgalone::client
