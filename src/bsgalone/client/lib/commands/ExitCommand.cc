
#include "ExitCommand.hh"

namespace bsgalone::client {

ExitCommand::ExitCommand()
  : IUiCommand(UiCommandType::EXIT_REQUESTED)
{}

auto ExitCommand::clone() const -> IUiCommandPtr
{
  return std::make_unique<ExitCommand>();
}

} // namespace bsgalone::client
