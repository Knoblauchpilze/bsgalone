
#pragma once

#include "IEventBus.hh"
#include <condition_variable>
#include <mutex>
#include <vector>

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

  /// @brief - Will block the calling thread until at least one event is
  /// received.
  /// This function can either block in case there are no available events
  /// yet or return immediately if there are. It will not wait longer than
  /// a predefined duration to allow hanging tests in case events are not
  /// received.
  /// When it succeeds, the function returns all available events.
  /// @return - the received events
  auto waitForEvents() -> std::vector<net::IEventPtr>;

  /// @brief - Wraps a call to `waitForEvents` and expects a single event
  /// to be returned. In case more than one or none is returned an error
  /// will be raised.
  /// @return - the single event that was received
  auto waitForEvent() -> net::IEventPtr;

  private:
  std::mutex m_locker{};
  std::condition_variable m_notifier{};

  std::vector<net::IEventPtr> m_events{};
};

} // namespace test
