
#include "EventProcessor.hh"
#include "ClientConnectedEvent.hh"
#include "DataReceivedEvent.hh"
#include <chrono>
#include <future>
#include <gtest/gtest.h>

using namespace ::testing;

namespace net {

TEST(Unit_Net_Messaging_EventProcessor, DoesNotCallHandlerWhenNoMessagesAreAvailable)
{
  std::deque<IEventPtr> noEvents{};
  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const IEvent & /*event*/) { control.fetch_add(1); };

  EventProcessor processor("test", noEvents, locker, handler);

  processor.processEvents();

  EXPECT_EQ(0, control.load());
}

TEST(Unit_Net_Messaging_EventProcessor, CallsHandlerForEachAvailableMessage)
{
  std::vector<char> sampleData{'t', 'e', 's', 't'};
  std::deque<IEventPtr> events;
  events.push_back(std::make_unique<ClientConnectedEvent>(ClientId{2}));
  events.push_back(std::make_unique<DataReceivedEvent>(ClientId{3}, sampleData));
  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const IEvent & /*event*/) { control.fetch_add(1); };

  EventProcessor processor("test", events, locker, handler);

  processor.processEvents();

  EXPECT_EQ(2, control.load());
}

TEST(Unit_Net_Messaging_EventProcessor, ForwardsAvailableMessageToHandler)
{
  std::vector<char> sampleData{'t', 'e', 's', 't'};
  std::deque<IEventPtr> events;
  events.push_back(std::make_unique<DataReceivedEvent>(ClientId{3}, sampleData));

  std::mutex locker{};

  auto handler = [&sampleData](const IEvent &event) {
    EXPECT_EQ(EventType::DATA_RECEIVED, event.type());

    const auto &actual = event.as<DataReceivedEvent>();
    EXPECT_EQ(ClientId{3}, actual.clientId());
    EXPECT_EQ(sampleData, actual.data());
  };

  EventProcessor processor("test", events, locker, handler);

  processor.processEvents();
}

TEST(Unit_Net_Messaging_EventProcessor, AcquiresProvidedLockBeforeProcessing)
{
  std::vector<char> sampleData{'t', 'e', 's', 't'};
  std::deque<IEventPtr> events;
  events.push_back(std::make_unique<DataReceivedEvent>(ClientId{3}, sampleData));

  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const IEvent & /*event*/) { control.fetch_add(1); };

  // Lock the mutex to prevent processing
  locker.lock();

  EventProcessor processor("test", events, locker, handler);

  // Trigger the processing of the events in a separate thread so that we
  // can verify that the handler is not called while the thread is locked
  auto started       = std::promise<bool>();
  auto threadStarted = started.get_future();

  auto result = std::async(std::launch::async, [&processor, &started]() {
    started.set_value(true);
    processor.processEvents();
  });
  threadStarted.get();

  EXPECT_EQ(0, control.load());

  // Release the mutex: this should allow event processing to happen
  locker.unlock();

  // We should have received a call to `processEvents`
  result.get();
  EXPECT_EQ(1, control.load());
}

} // namespace net
