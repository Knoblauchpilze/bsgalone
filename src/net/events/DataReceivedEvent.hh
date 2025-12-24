
#pragma once

#include "ClientId.hh"
#include "IEvent.hh"
#include <vector>

namespace net {

class DataReceivedEvent : public IEvent
{
  public:
  DataReceivedEvent(const ClientId clientId, std::vector<char> data);
  ~DataReceivedEvent() override = default;

  auto clone() const -> IEventPtr override;

  private:
  ClientId m_clientId{};
  std::vector<char> m_data{};
};

} // namespace net
