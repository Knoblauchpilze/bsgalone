
#include "DataSentEvent.hh"

namespace net {

DataSentEvent::DataSentEvent(const std::optional<ClientId> &clientId, const MessageId messageId)
  : INetworkEvent(NetworkEventType::DATA_SENT)
  , m_clientId(clientId)
  , m_messageId(messageId)
{}

auto DataSentEvent::tryGetClientId() const -> std::optional<ClientId>
{
  return m_clientId;
}

auto DataSentEvent::messageId() const -> MessageId
{
  return m_messageId;
}

auto DataSentEvent::clone() const -> INetworkEventPtr
{
  return std::make_unique<DataSentEvent>(m_clientId, m_messageId);
}

} // namespace net
