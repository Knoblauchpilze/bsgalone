
#pragma once

#include "EventTypeConcept.hh"
#include <memory>

namespace messaging {

template<EventTypeLike EventType>
class AbstractEvent
{
  public:
  virtual ~AbstractEvent() = default;

  auto type() const -> EventType;

  virtual auto clone() const -> std::unique_ptr<AbstractEvent> = 0;

  template<typename Event>
  auto as() -> Event &;

  template<typename Event>
  auto as() const -> const Event &;

  template<typename Event>
  bool isA() const;

  protected:
  AbstractEvent(const EventType type);

  EventType m_type{};
};

} // namespace messaging

#include "AbstractEvent.hxx"
