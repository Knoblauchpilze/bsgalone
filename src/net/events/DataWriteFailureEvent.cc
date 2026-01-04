
#include "DataWriteFailureEvent.hh"

namespace net {

DataWriteFailureEvent::DataWriteFailureEvent(const ClientId clientId)
  : IEvent(EventType::DATA_WRITE_FAILURE)
  , m_clientId(clientId)
{}

auto DataWriteFailureEvent::clientId() const -> ClientId
{
  return m_clientId;
}

auto DataWriteFailureEvent::clone() const -> IEventPtr
{
  return std::make_unique<DataWriteFailureEvent>(m_clientId);
}

} // namespace net
