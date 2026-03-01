
#pragma once

#include "CoreObject.hh"
#include "EventConcept.hh"
#include "EventTypeConcept.hh"
#include "IEventQueue.hh"
#include <deque>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace messaging {

template<EventTypeLike EventType, EventLike<EventType> Event>
class AbstractSynchronizedEventQueue : public IEventQueue<EventType, Event>, public core::CoreObject
{
  public:
  AbstractSynchronizedEventQueue(std::unordered_set<EventType> allEvents,
                                 std::unordered_set<EventType> eventsToFilterForLogging);
  ~AbstractSynchronizedEventQueue() override = default;

  using Base = IEventQueue<EventType, Event>;

  /// @brief - Atomically pushes an event to be processed by this queue. It won't interfere
  /// with existing calls to `processEvents`. In case processing events is already happening,
  /// the event will only be processed on the next call to `processEvents`.
  /// This method takes ownership of the event (as indicating by the unique pointer).
  /// @param event - the event to register
  void pushEvent(Base::EventPtr event) override;

  /// @brief - Registers a new listener to this event queue. Note that this method is not thread
  /// safe: calling it when the `processEvents` method is already running will cause undefined
  /// behavior.
  /// This is because this class uses the `AbstractEventProcessor` which only copies events and
  /// not the listeners. Making it thread safe would mean copying the listeners as well.
  /// It is recommended to only add listeners before starting a loop/thread to process events
  /// continuously.
  /// @param listener - the new listener to register
  void addListener(Base::EventListenerPtr listener) override;

  /// @brief - Atomically verifies whether there are events to be processed. This method will
  /// return true as soon as the `processEvents` has been called, even if all events have not
  /// yet been processed.
  bool empty() override;

  /// @brief - Processes the events received by the queue in a thread-safe way. This means that
  /// it is safe to push new events while this method is running.
  void processEvents() override;

  private:
  std::unordered_set<EventType> m_allEvents{};
  std::unordered_set<EventType> m_loggingFilter{};

  std::vector<typename Base::EventListenerPtr> m_listeners{};
  std::unordered_multimap<EventType, typename Base::EventListener *> m_listenersTable{};

  std::mutex m_locker{};
  std::deque<typename Base::EventPtr> m_events{};

  void processEvent(const Event &event) const;
};

} // namespace messaging

#include "AbstractSynchronizedEventQueue.hxx"
