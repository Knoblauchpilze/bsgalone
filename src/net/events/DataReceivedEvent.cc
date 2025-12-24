
#include "DataReceivedEvent.hh"

namespace net {

DataReceivedEvent::DataReceivedEvent(const ClientId clientId, std::vector<char> data)
  : IEvent(EventType::DATA_RECEIVED)
  , m_clientId(clientId)
  , m_data(std::move(data))
{}

auto DataReceivedEvent::clone() const -> IEventPtr
{
  return std::make_unique<DataReceivedEvent>(m_clientId, m_data);
}

} // namespace net
