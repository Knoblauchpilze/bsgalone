
#pragma once

#include "ClientId.hh"
#include "IEvent.hh"

namespace net {

class DataWriteFailureEvent : public IEvent
{
  public:
  DataWriteFailureEvent(const ClientId clientId);
  ~DataWriteFailureEvent() override = default;

  auto clientId() const -> ClientId;

  auto clone() const -> IEventPtr override;

  private:
  ClientId m_clientId{};
};

} // namespace net
