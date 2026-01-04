
#include "DataReadFailureEvent.hh"

namespace net {

DataReadFailureEvent::DataReadFailureEvent(const ClientId clientId)
  : IEvent(EventType::DATA_READ_FAILURE)
  , m_clientId(clientId)
{}

auto DataReadFailureEvent::clientId() const -> ClientId
{
  return m_clientId;
}

auto DataReadFailureEvent::clone() const -> IEventPtr
{
  return std::make_unique<DataReadFailureEvent>(m_clientId);
}

} // namespace net
