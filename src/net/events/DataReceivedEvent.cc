
#include "DataReceivedEvent.hh"

namespace net {

DataReceivedEvent::DataReceivedEvent(const std::optional<ClientId> &clientId, std::vector<char> data)
  : IEvent(EventType::DATA_RECEIVED)
  , m_clientId(clientId)
  , m_data(std::move(data))
{}

auto DataReceivedEvent::tryGetClientId() const -> std::optional<ClientId>
{
  return m_clientId;
}

auto DataReceivedEvent::data() const -> std::vector<char>
{
  return m_data;
}

auto DataReceivedEvent::clone() const -> IEventPtr
{
  return std::make_unique<DataReceivedEvent>(m_clientId, m_data);
}

} // namespace net
