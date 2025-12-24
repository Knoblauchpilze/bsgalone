
#pragma once

#include "IEvent.hh"

namespace net {

template<typename Event>
inline auto IEvent::as() -> Event &
{
  return dynamic_cast<Event &>(*this);
}

template<typename Event>
inline auto IEvent::as() const -> const Event &
{
  return dynamic_cast<const Event &>(*this);
}

template<typename Event>
inline bool IEvent::isA() const
{
  return dynamic_cast<const Event *>(this) != nullptr;
}

} // namespace net
