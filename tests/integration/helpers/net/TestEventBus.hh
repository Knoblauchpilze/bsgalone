
#pragma once

#include "IEventBus.hh"
#include <condition_variable>
#include <memory>
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

  /// @brief - Used to wait until an event with the specified type is received.
  /// In case other events are received they will be ignored. This method will
  /// retry until `maxTries` tries have been performed.
  /// As a general rule, if it is expected that X events can be produced and
  /// the desired event is among those, the `maxTries` should be equal to the
  /// total number of possible events.
  /// In case the event is not received in time, an error will be raised.
  /// @param type - the type of event to wait for
  /// @param maxTries - the maximum number of tries to perform when waiting
  /// for an event
  /// @return - the event with the desired type
  auto waitForEvent(const net::EventType type, const int maxTries = 2) -> net::IEventPtr;

  private:
  std::mutex m_locker{};
  std::condition_variable m_notifier{};

  std::vector<net::IEventPtr> m_events{};
};

using TestEventBusShPtr = std::shared_ptr<TestEventBus>;

} // namespace test
