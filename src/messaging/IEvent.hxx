
#pragma once

#include "IGameEvent.hh"

namespace bsgalone::core {

template<typename Event>
inline auto IGameEvent::as() -> Event &
{
  return dynamic_cast<Event &>(*this);
}

template<typename Event>
inline auto IGameEvent::as() const -> const Event &
{
  return dynamic_cast<const Event &>(*this);
}

template<typename Event>
inline bool IGameEvent::isA() const
{
  return dynamic_cast<const Event *>(this) != nullptr;
}

} // namespace bsgalone::core
