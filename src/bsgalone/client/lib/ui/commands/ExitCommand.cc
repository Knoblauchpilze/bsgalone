
#include "ExitCommand.hh"

namespace bsgalone::client {

ExitCommand::ExitCommand()
  : IUiEvent(UiEventType::EXIT_REQUESTED)
{}

auto ExitCommand::clone() const -> IUiEventPtr
{
  return std::make_unique<ExitCommand>();
}

} // namespace bsgalone::client
