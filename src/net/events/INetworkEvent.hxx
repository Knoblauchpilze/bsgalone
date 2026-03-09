
#pragma once

#include "INetworkEvent.hh"

namespace net {

template<typename Event>
inline auto INetworkEvent::as() -> Event &
{
  return dynamic_cast<Event &>(*this);
}

template<typename Event>
inline auto INetworkEvent::as() const -> const Event &
{
  return dynamic_cast<const Event &>(*this);
}

template<typename Event>
inline bool INetworkEvent::isA() const
{
  return dynamic_cast<const Event *>(this) != nullptr;
}

} // namespace net
