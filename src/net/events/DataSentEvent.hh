
#pragma once

#include "ClientId.hh"
#include "IEvent.hh"

namespace net {

class DataSentEvent : public IEvent
{
  public:
  DataSentEvent(const ClientId clientId);
  ~DataSentEvent() override = default;

  auto clone() const -> IEventPtr override;

  private:
  ClientId m_clientId{};
};

} // namespace net
