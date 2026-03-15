
#include "JumpRequestedCommand.hh"

namespace bsgalone::client {

JumpRequestedCommand::JumpRequestedCommand()
  : IUiEvent(UiEventType::JUMP_REQUESTED)
{}

auto JumpRequestedCommand::clone() const -> IUiEventPtr
{
  return std::make_unique<JumpRequestedCommand>();
}

} // namespace bsgalone::client
