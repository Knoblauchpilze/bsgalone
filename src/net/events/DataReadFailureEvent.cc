
#include "DataReadFailureEvent.hh"

namespace net {

DataReadFailureEvent::DataReadFailureEvent(const std::optional<ClientId> &clientId)
  : INetworkEvent(NetworkEventType::DATA_READ_FAILURE)
  , m_clientId(clientId)
{}

auto DataReadFailureEvent::tryGetClientId() const -> std::optional<ClientId>
{
  return m_clientId;
}

auto DataReadFailureEvent::clone() const -> INetworkEventPtr
{
  return std::make_unique<DataReadFailureEvent>(m_clientId);
}

} // namespace net
