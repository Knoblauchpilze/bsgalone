
#pragma once

#include "ClientId.hh"
#include "IEvent.hh"
#include "MessageId.hh"
#include <optional>

namespace net {

class DataSentEvent : public IEvent
{
  public:
  DataSentEvent(const std::optional<ClientId> &clientId, const MessageId messageId);
  ~DataSentEvent() override = default;

  auto tryGetClientId() const -> std::optional<ClientId>;
  auto messageId() const -> MessageId;

  auto clone() const -> IEventPtr override;

  private:
  std::optional<ClientId> m_clientId{};
  MessageId m_messageId{};
};

} // namespace net
