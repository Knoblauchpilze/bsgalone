
#pragma once

#include <string>
#include <unordered_set>

namespace net {

enum class NetworkEventType
{
  CONNECTION_ESTABLISHED,
  CONNECTION_LOST,
  CLIENT_CONNECTED,
  CLIENT_DISCONNECTED,
  DATA_READ_FAILURE,
  DATA_RECEIVED,
  DATA_SENT,
  DATA_WRITE_FAILURE,
  SERVER_STARTED,
  SERVER_STOPPED
};

auto str(const NetworkEventType &type) -> std::string;

auto allNetworkEventTypesAsSet() -> std::unordered_set<NetworkEventType>;

} // namespace net
