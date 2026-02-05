
#pragma once

#include "ClientId.hh"
#include "IEvent.hh"
#include <optional>
#include <vector>

namespace net {

class DataReceivedEvent : public IEvent
{
  public:
  DataReceivedEvent(const std::optional<ClientId> &clientId, std::vector<char> data);
  ~DataReceivedEvent() override = default;

  auto tryGetClientId() const -> std::optional<ClientId>;
  auto data() const -> std::vector<char>;

  auto clone() const -> IEventPtr override;

  private:
  std::optional<ClientId> m_clientId{};
  std::vector<char> m_data{};
};

} // namespace net
