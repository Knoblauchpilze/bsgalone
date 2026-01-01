
#include "DataSentEvent.hh"

namespace net {

DataSentEvent::DataSentEvent(const ClientId clientId)
  : IEvent(EventType::DATA_SENT)
  , m_clientId(clientId)
{}

auto DataSentEvent::clientId() const -> ClientId
{
  return m_clientId;
}

auto DataSentEvent::clone() const -> IEventPtr
{
  return std::make_unique<DataSentEvent>(m_clientId);
}

} // namespace net
