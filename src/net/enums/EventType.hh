
#pragma once

#include <string>
#include <unordered_set>

namespace net {

enum class EventType
{
  CLIENT_CONNECTED,
  CLIENT_DISCONNECTED,
  DATA_READ_FAILURE,
  DATA_RECEIVED,
  DATA_SENT,
  DATA_WRITE_FAILURE,
  SERVER_STARTED,
  SERVER_STOPPED
};

auto str(const EventType &type) -> std::string;

auto allEventTypesAsSet() -> std::unordered_set<EventType>;

} // namespace net
