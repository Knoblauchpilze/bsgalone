
#pragma once

#include "EventType.hh"
#include <memory>

namespace net {

class INetworkEvent
{
  public:
  virtual ~INetworkEvent() = default;

  auto type() const -> EventType;

  virtual auto clone() const -> std::unique_ptr<INetworkEvent> = 0;

  template<typename Event>
  auto as() -> Event &;

  template<typename Event>
  auto as() const -> const Event &;

  template<typename Event>
  bool isA() const;

  protected:
  INetworkEvent(const EventType type);

  private:
  EventType m_type{};
};

using INetworkEventPtr = std::unique_ptr<INetworkEvent>;

} // namespace net

#include "INetworkEvent.hxx"
