
#pragma once

#include "ClientId.hh"
#include "IEvent.hh"
#include "MessageId.hh"

namespace net {

class DataSentEvent : public IEvent
{
  public:
  DataSentEvent(const ClientId clientId, const MessageId messageId);
  ~DataSentEvent() override = default;

  auto clientId() const -> ClientId;
  auto messageId() const -> MessageId;

  auto clone() const -> IEventPtr override;

  private:
  ClientId m_clientId{};
  MessageId m_messageId{};
};

} // namespace net
