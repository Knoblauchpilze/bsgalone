
#include "EventType.hh"

namespace net {

auto str(const EventType &type) -> std::string
{
  switch (type)
  {
    case EventType::CONNECTION_ESTABLISHED:
      return "connection_established";
    case EventType::CONNECTION_LOST:
      return "connection_lost";
    case EventType::CLIENT_CONNECTED:
      return "client_connected";
    case EventType::CLIENT_DISCONNECTED:
      return "client_disconnected";
    case EventType::DATA_READ_FAILURE:
      return "data_read_failure";
    case EventType::DATA_RECEIVED:
      return "data_received";
    case EventType::DATA_SENT:
      return "data_sent";
    case EventType::DATA_WRITE_FAILURE:
      return "data_write_failure";
    case EventType::SERVER_STARTED:
      return "server_started";
    case EventType::SERVER_STOPPED:
      return "server_stopped";
    default:
      return "unknown";
  }
}

auto allEventTypesAsSet() -> std::unordered_set<EventType>
{
  return std::unordered_set<EventType>{
    EventType::CONNECTION_ESTABLISHED,
    EventType::CONNECTION_LOST,
    EventType::CLIENT_CONNECTED,
    EventType::CLIENT_DISCONNECTED,
    EventType::DATA_READ_FAILURE,
    EventType::DATA_RECEIVED,
    EventType::DATA_SENT,
    EventType::DATA_WRITE_FAILURE,
    EventType::SERVER_STARTED,
    EventType::SERVER_STOPPED,
  };
}

} // namespace net
