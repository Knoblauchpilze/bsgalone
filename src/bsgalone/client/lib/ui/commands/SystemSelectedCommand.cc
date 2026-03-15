
#include "SystemSelectedCommand.hh"

namespace bsgalone::client {

SystemSelectedCommand::SystemSelectedCommand(const core::Uuid systemDbId)
  : IUiEvent(UiEventType::JUMP_REQUESTED)
  , m_systemDbId(systemDbId)
{}

auto SystemSelectedCommand::getSystemDbId() const -> core::Uuid
{
  return m_systemDbId;
}

auto SystemSelectedCommand::clone() const -> IUiEventPtr
{
  return std::make_unique<SystemSelectedCommand>(m_systemDbId);
}

} // namespace bsgalone::client
