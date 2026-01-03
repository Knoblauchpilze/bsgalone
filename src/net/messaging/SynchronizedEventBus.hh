
#pragma once

#include "AbstractEventBus.hh"
#include "CoreObject.hh"
#include <deque>
#include <mutex>

namespace net {

class SynchronizedEventBus : public AbstractEventBus, public core::CoreObject
{
  public:
  SynchronizedEventBus();
  ~SynchronizedEventBus() override = default;

  /// @brief - Atomically pushes an event to be processed by this bus. It won't interfere with
  /// existing calls to `processEvents`. In case processing events is already happening, the
  /// event will only be processed on the next call to `processEvents`.
  /// This method takes ownership of the event (as indicating by the unique pointer).
  /// @param event - the event to refister
  void pushEvent(IEventPtr event) override;

  /// @brief - Registers a new listener to this event bus. Note that this method is not thread
  /// safe: calling it when the `processEvents` method is already running will cause undefined
  /// behavior.
  /// This is because this class uses the `EventProcessor` which only copies events and not the
  /// listeners. Making it thread safe would mean copying the listeners as well.
  /// It is recommended to only add listeners before starting a loop/thread to process events
  /// continuously.
  /// @param listener - the new listener to register
  void addListener(IEventListenerPtr listener) override;

  /// @brief - Atomically verifies whether there are events to be processed. This method will
  /// return true as soon as the `processEvents` has been called, even if all events have not
  /// yet been processed.
  bool empty() override;

  /// @brief - Processes the events received by the bus in a thread-safe way. This means that
  /// it is safe to push new events while this method is running.
  void processEvents() override;

  private:
  std::mutex m_locker{};
  std::deque<IEventPtr> m_events{};

  void processEvent(const IEvent &event) const;
};

} // namespace net
