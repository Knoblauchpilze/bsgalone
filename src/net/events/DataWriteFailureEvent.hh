
#pragma once

#include "ClientId.hh"
#include "INetworkEvent.hh"
#include "MessageId.hh"
#include <optional>

namespace net {

class DataWriteFailureEvent : public INetworkEvent
{
  public:
  DataWriteFailureEvent(const std::optional<ClientId> &clientId, const MessageId messageId);
  ~DataWriteFailureEvent() override = default;

  auto tryGetClientId() const -> std::optional<ClientId>;
  auto messageId() const -> MessageId;

  auto clone() const -> INetworkEventPtr override;

  private:
  std::optional<ClientId> m_clientId{};
  MessageId m_messageId{};
};

} // namespace net
