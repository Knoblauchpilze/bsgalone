
#pragma once

#include "ClientId.hh"
#include "IEvent.hh"
#include <optional>

namespace net {

class DataReadFailureEvent : public IEvent
{
  public:
  DataReadFailureEvent(const std::optional<ClientId> &clientId);
  ~DataReadFailureEvent() override = default;

  auto tryGetClientId() const -> std::optional<ClientId>;

  auto clone() const -> IEventPtr override;

  private:
  std::optional<ClientId> m_clientId{};
};

} // namespace net
