
#pragma once

#include "IEventBus.hh"
#include <deque>
#include <future>

namespace test {

class TestEventBus : public net::IEventBus
{
  public:
  TestEventBus()           = default;
  ~TestEventBus() override = default;

  void pushEvent(net::IEventPtr event) override;
  void addListener(net::IEventListenerPtr listener) override;
  bool empty() override;

  void processEvents() override;

  /// @brief - Will block the calling thread until an event is received.
  /// Calling this function multiple times before it returns will cause
  /// undefined behavior.
  /// @return - the received event
  auto waitForEvent() -> net::IEventPtr;

  private:
  std::promise<bool> m_received{};
  std::deque<net::IEventPtr> m_events{};
};

} // namespace test
