
#include "SystemsListReceivedEvent.hh"

namespace bsgalone::client {

SystemsListReceivedEvent::SystemsListReceivedEvent(const std::vector<core::System> &systemsData)
  : IUiEvent(UiEventType::SYSTEMS_LIST_RECEIVED)
  , m_systems(systemsData)
{}

auto SystemsListReceivedEvent::getSystems() const -> const std::vector<core::System> &
{
  return m_systems;
}

auto SystemsListReceivedEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<SystemsListReceivedEvent>(m_systems);
}

} // namespace bsgalone::client
