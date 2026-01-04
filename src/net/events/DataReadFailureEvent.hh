
#pragma once

#include "ClientId.hh"
#include "IEvent.hh"

namespace net {

class DataReadFailureEvent : public IEvent
{
  public:
  DataReadFailureEvent(const ClientId clientId);
  ~DataReadFailureEvent() override = default;

  auto clientId() const -> ClientId;

  auto clone() const -> IEventPtr override;

  private:
  ClientId m_clientId{};
};

} // namespace net
