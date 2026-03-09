
#pragma once

#include "ClientId.hh"
#include "INetworkEvent.hh"
#include <optional>

namespace net {

class DataReadFailureEvent : public INetworkEvent
{
  public:
  DataReadFailureEvent(const std::optional<ClientId> &clientId);
  ~DataReadFailureEvent() override = default;

  auto tryGetClientId() const -> std::optional<ClientId>;

  auto clone() const -> INetworkEventPtr override;

  private:
  std::optional<ClientId> m_clientId{};
};

} // namespace net
