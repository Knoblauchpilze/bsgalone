
#pragma once

#include "EventType.hh"
#include <memory>

namespace net {

class IEvent
{
  public:
  virtual ~IEvent() = default;

  auto type() const -> EventType;

  virtual auto clone() const -> std::unique_ptr<IEvent> = 0;

  template<typename Event>
  auto as() -> Event &;

  template<typename Event>
  auto as() const -> const Event &;

  template<typename Event>
  bool isA() const;

  protected:
  IEvent(const EventType type);

  private:
  EventType m_type{};
};

using IEventPtr = std::unique_ptr<IEvent>;

} // namespace net

#include "IEvent.hxx"
