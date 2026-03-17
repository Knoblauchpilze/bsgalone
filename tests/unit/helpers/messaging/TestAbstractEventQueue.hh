
#pragma once

#include "IEventQueue.hh"
#include <mutex>
#include <vector>

namespace test {

template<messaging::EventTypeLike EventType, messaging::EventLike<EventType> Event>
class TestAbstractEventQueue : public messaging::IEventQueue<EventType, Event>
{
  public:
  TestAbstractEventQueue()           = default;
  ~TestAbstractEventQueue() override = default;

  using Base = messaging::IEventQueue<EventType, Event>;

  void pushEvent(Base::EventPtr message) override;
  void addListener(Base::EventListenerPtr /*listener*/) override;
  bool empty() override;
  void processEvents() override;

  /// @brief - Returns the accumulated messages so far. Note that this method
  /// is **not thread-safe**.
  /// It is intended to be used after all events have been successfully added
  /// to the queue.
  /// @return - the list of accumulated messages
  auto messages() const -> const std::vector<typename Base::EventPtr> &;

  void clearMessages();

  private:
  std::mutex m_locker{};
  std::vector<typename Base::EventPtr> m_messages{};
};

} // namespace test

#include "TestAbstractEventQueue.hxx"
