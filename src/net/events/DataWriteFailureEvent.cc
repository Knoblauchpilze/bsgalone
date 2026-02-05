
#include "DataWriteFailureEvent.hh"

namespace net {

DataWriteFailureEvent::DataWriteFailureEvent(const std::optional<ClientId> &clientId,
                                             const MessageId messageId)
  : IEvent(EventType::DATA_WRITE_FAILURE)
  , m_clientId(clientId)
  , m_messageId(messageId)
{}

auto DataWriteFailureEvent::tryGetClientId() const -> std::optional<ClientId>
{
  return m_clientId;
}

auto DataWriteFailureEvent::messageId() const -> MessageId
{
  return m_messageId;
}

auto DataWriteFailureEvent::clone() const -> IEventPtr
{
  return std::make_unique<DataWriteFailureEvent>(m_clientId, m_messageId);
}

} // namespace net
