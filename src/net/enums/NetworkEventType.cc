
#include "NetworkEventType.hh"

namespace net {

auto str(const NetworkEventType &type) -> std::string
{
  switch (type)
  {
    case NetworkEventType::CONNECTION_ESTABLISHED:
      return "connection_established";
    case NetworkEventType::CONNECTION_LOST:
      return "connection_lost";
    case NetworkEventType::CLIENT_CONNECTED:
      return "client_connected";
    case NetworkEventType::CLIENT_DISCONNECTED:
      return "client_disconnected";
    case NetworkEventType::DATA_READ_FAILURE:
      return "data_read_failure";
    case NetworkEventType::DATA_RECEIVED:
      return "data_received";
    case NetworkEventType::DATA_SENT:
      return "data_sent";
    case NetworkEventType::DATA_WRITE_FAILURE:
      return "data_write_failure";
    case NetworkEventType::SERVER_STARTED:
      return "server_started";
    case NetworkEventType::SERVER_STOPPED:
      return "server_stopped";
    default:
      return "unknown";
  }
}

auto allNetworkEventTypesAsSet() -> std::unordered_set<NetworkEventType>
{
  return std::unordered_set<NetworkEventType>{
    NetworkEventType::CONNECTION_ESTABLISHED,
    NetworkEventType::CONNECTION_LOST,
    NetworkEventType::CLIENT_CONNECTED,
    NetworkEventType::CLIENT_DISCONNECTED,
    NetworkEventType::DATA_READ_FAILURE,
    NetworkEventType::DATA_RECEIVED,
    NetworkEventType::DATA_SENT,
    NetworkEventType::DATA_WRITE_FAILURE,
    NetworkEventType::SERVER_STARTED,
    NetworkEventType::SERVER_STOPPED,
  };
}

} // namespace net
