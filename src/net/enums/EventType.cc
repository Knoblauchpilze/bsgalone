
#include "EventType.hh"

namespace net {

auto str(const EventType &type) -> std::string
{
  switch (type)
  {
    case EventType::CLIENT_CONNECTED:
      return "client_connected";
    case EventType::CLIENT_DISCONNECTED:
      return "client_disconnected";
    case EventType::DATA_RECEIVED:
      return "data_received";
    case EventType::DATA_SENT:
      return "data_sent";
    default:
      return "unknown";
  }
}

auto allEventTypesAsSet() -> std::unordered_set<EventType>
{
  return std::unordered_set<EventType>{
    EventType::CLIENT_CONNECTED,
    EventType::CLIENT_DISCONNECTED,
    EventType::DATA_RECEIVED,
    EventType::DATA_SENT,
  };
}

} // namespace net
