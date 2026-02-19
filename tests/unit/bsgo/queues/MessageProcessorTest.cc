
#include "MessageProcessor.hh"
#include "ConnectionMessage.hh"
#include "DockMessage.hh"
#include <chrono>
#include <future>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {

TEST(Unit_Bsgo_Queues_MessageProcessor, DoesNotCallHandlerWhenNoMessagesAreAvailable)
{
  std::deque<bsgalone::core::IMessagePtr> noMessages{};
  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const bsgalone::core::IMessage & /*message*/) { control.fetch_add(1); };

  MessageProcessor processor("test", noMessages, locker, handler);

  processor.processMessages();

  EXPECT_EQ(0, control.load());
}

TEST(Unit_Bsgo_Queues_MessageProcessor, CallsHandlerForEachAvailableMessage)
{
  std::vector<char> sampleData{'t', 'e', 's', 't'};
  std::deque<bsgalone::core::IMessagePtr> messages;
  messages.push_back(std::make_unique<ConnectionMessage>());
  messages.push_back(
    std::make_unique<bsgalone::core::DockMessage>(Uuid{18},
                                                  Uuid{19},
                                                  Uuid{3},
                                                  bsgalone::core::DockTransition::UNDOCK));
  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const bsgalone::core::IMessage & /*message*/) { control.fetch_add(1); };

  MessageProcessor processor("test", messages, locker, handler);

  processor.processMessages();

  EXPECT_EQ(2, control.load());
}

TEST(Unit_Bsgo_Queues_MessageProcessor, ForwardsAvailableMessageToHandler)
{
  std::deque<bsgalone::core::IMessagePtr> messages;
  messages.push_back(
    std::make_unique<bsgalone::core::DockMessage>(Uuid{18},
                                                  Uuid{19},
                                                  Uuid{3},
                                                  bsgalone::core::DockTransition::UNDOCK));

  std::mutex locker{};

  auto handler = [](const bsgalone::core::IMessage &message) {
    EXPECT_EQ(bsgalone::core::MessageType::DOCK, message.type());

    const auto &actual = message.as<bsgalone::core::DockMessage>();
    EXPECT_EQ(Uuid{3}, actual.getShipDbId());
    EXPECT_EQ(bsgalone::core::DockTransition::UNDOCK, actual.getTransition());
  };

  MessageProcessor processor("test", messages, locker, handler);

  processor.processMessages();
}

TEST(Unit_Bsgo_Queues_MessageProcessor, AcquiresProvidedLockBeforeProcessing)
{
  std::deque<bsgalone::core::IMessagePtr> messages;
  messages.push_back(std::make_unique<ConnectionMessage>());

  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const bsgalone::core::IMessage & /*message*/) { control.fetch_add(1); };

  // Lock the mutex to prevent processing
  locker.lock();

  MessageProcessor processor("test", messages, locker, handler);

  // Trigger the processing of the messages in a separate thread so that we
  // can verify that the handler is not called while the thread is locked
  auto started       = std::promise<bool>();
  auto threadStarted = started.get_future();

  auto result = std::async(std::launch::async, [&processor, &started]() {
    started.set_value(true);
    processor.processMessages();
  });
  threadStarted.get();

  EXPECT_EQ(0, control.load());

  // Release the mutex: this should allow messages processing to happen
  locker.unlock();

  // We should have received a call to `processMessages`
  result.get();
  EXPECT_EQ(1, control.load());
}

} // namespace bsgo
