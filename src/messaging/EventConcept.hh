
#pragma once

#include <concepts>
#include <string>

namespace messaging {

template<typename Event, typename EventType>
concept EventLike = std::is_enum_v<EventType> && requires(Event e) {
  {
    e.type()
  } -> std::same_as<EventType>;
};

} // namespace messaging
