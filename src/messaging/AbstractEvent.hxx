
#pragma once

#include "AbstractEvent.hh"

namespace messaging {

template<EventTypeLike EventType>
AbstractEvent<EventType>::AbstractEvent(const EventType type)
  : m_type(type)
{}

template<EventTypeLike EventType>
auto AbstractEvent<EventType>::type() const -> EventType
{
  return m_type;
}

template<EventTypeLike EventType>
template<typename Event>
inline auto AbstractEvent<EventType>::as() -> Event &
{
  return dynamic_cast<Event &>(*this);
}

template<EventTypeLike EventType>
template<typename Event>
inline auto AbstractEvent<EventType>::as() const -> const Event &
{
  return dynamic_cast<const Event &>(*this);
}

template<EventTypeLike EventType>
template<typename Event>
inline bool AbstractEvent<EventType>::isA() const
{
  return dynamic_cast<const Event *>(this) != nullptr;
}

} // namespace messaging
