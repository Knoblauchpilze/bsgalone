
#include "NetworkAdapter.hh"
#include "DataReceivedEvent.hh"
#include "LootMessage.hh"
#include "ScannedMessage.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
class TestMessageQueue : public bsgo::IMessageQueue
{
  public:
  TestMessageQueue()           = default;
  ~TestMessageQueue() override = default;

  void pushMessage(bsgo::IMessagePtr message) override
  {
    const std::lock_guard guard(m_locker);
    m_messages.push_back(std::move(message));
  }

  void addListener(bsgo::IMessageListenerPtr /*listener*/) override
  {
    throw std::runtime_error("Unsupported operation addListener in TestMessageQueue");
  }

  bool empty() override
  {
    const std::lock_guard guard(m_locker);
    return m_messages.empty();
  }

  void processMessages() override
  {
    throw std::runtime_error("Unsupported operation processMessages in TestMessageQueue");
  }

  /// @brief - Returns the accumulated messages so far. Note that this method
  /// is **not thread-safe**.
  /// It is intended to be used after all events have been successfully added
  /// to the queue.
  /// @return - the list of accumulated messages
  auto messages() const -> const std::vector<bsgo::IMessagePtr> &
  {
    return m_messages;
  }

  void clearMessages()
  {
    const std::lock_guard guard(m_locker);
    return m_messages.clear();
  }

  private:
  std::mutex m_locker{};
  std::vector<bsgo::IMessagePtr> m_messages{};
};

auto generateCompleteScannedMessage() -> std::vector<char>
{
  bsgo::ScannedMessage message(bsgo::Uuid{2}, bsgo::Uuid{4});
  std::stringstream out;
  message.serialize(out);
  const auto serialized = out.str();
  return std::vector<char>(serialized.begin(), serialized.end());
}
} // namespace

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { NetworkAdapter(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, CorrectlyPublishesCompleteMessage)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  const net::DataReceivedEvent event(net::ClientId{1}, data);

  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &message = queue->messages().at(0);
  EXPECT_EQ(bsgo::MessageType::SCANNED, message->type());
  EXPECT_EQ(net::ClientId{1}, message->as<bsgo::ScannedMessage>().getClientId());
  EXPECT_EQ(bsgo::Uuid{2}, message->as<bsgo::ScannedMessage>().getPlayerDbId());
  EXPECT_EQ(bsgo::Uuid{4}, message->as<bsgo::ScannedMessage>().getAsteroidDbId());
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, CorrectlyPublishesCompleteMessageWithNoClientId)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  const net::DataReceivedEvent event({}, data);

  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &message = queue->messages().at(0);
  EXPECT_EQ(bsgo::MessageType::SCANNED, message->type());
  EXPECT_FALSE(message->as<bsgo::ScannedMessage>().tryGetClientId().has_value());
  EXPECT_EQ(bsgo::Uuid{2}, message->as<bsgo::ScannedMessage>().getPlayerDbId());
  EXPECT_EQ(bsgo::Uuid{4}, message->as<bsgo::ScannedMessage>().getAsteroidDbId());
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, DoesNotPublishMessageWhenIncompleteDataIsReceived)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());
  const net::DataReceivedEvent event(net::ClientId{0},
                                     std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, PublishesMessageWhenAllDataIsReceived)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent(net::ClientId{0}, std::vector<char>(data.begin() + 5, data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &message = queue->messages().at(0);
  EXPECT_EQ(bsgo::MessageType::SCANNED, message->type());
  EXPECT_EQ(bsgo::Uuid{2}, message->as<bsgo::ScannedMessage>().getPlayerDbId());
  EXPECT_EQ(bsgo::Uuid{4}, message->as<bsgo::ScannedMessage>().getAsteroidDbId());
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, DoesNotPublishMessageTwice)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  net::DataReceivedEvent event(net::ClientId{0}, data);

  adapter.onEventReceived(event);
  EXPECT_FALSE(queue->empty());

  queue->clearMessages();

  event = net::DataReceivedEvent(net::ClientId{0}, std::vector<char>());
  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter,
     DoesNotPublishMessageWhenDataReceivedForAnotherClient)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent(net::ClientId{1},
                                 std::vector<char>(data.begin(), data.begin() + 5));
  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, DoesNotPublishMessageWhenDataReceivedForNoClient)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent({}, std::vector<char>(data.begin(), data.begin() + 5));
  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, OnlyPublishMessageForReceivedData)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());

  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin(), data.begin() + 5));
  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent(net::ClientId{1},
                                 std::vector<char>(data.begin(), data.begin() + 5));
  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent(net::ClientId{0}, std::vector<char>(data.begin() + 5, data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &message = queue->messages().at(0);
  EXPECT_EQ(bsgo::MessageType::SCANNED, message->type());
  EXPECT_EQ(bsgo::Uuid{2}, message->as<bsgo::ScannedMessage>().getPlayerDbId());
  EXPECT_EQ(bsgo::Uuid{4}, message->as<bsgo::ScannedMessage>().getAsteroidDbId());
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, ThrowsWhenReceivingInvalidData)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin() + 5, data.end()));

  auto body = [&adapter, &event]() { adapter.onEventReceived(event); };
  EXPECT_THROW(body(), ::core::CoreException);
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, SetsClientIdForNetworkMessageWhenNotDefined)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  bsgo::LootMessage message(bsgo::Uuid{2}, bsgo::Uuid{4}, 12);
  std::stringstream out;
  message.serialize(out);
  const auto serialized = out.str();
  std::vector<char> data(serialized.begin(), serialized.end());

  net::DataReceivedEvent event(net::ClientId{17}, std::vector<char>(data.begin(), data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(bsgo::MessageType::LOOT, actual->type());
  EXPECT_EQ(net::ClientId{17}, actual->as<bsgo::LootMessage>().getClientId());
}

TEST(Unit_Bsgalone_Core_Network_NetworkAdapter, OverridesExistingClientIdWithConnectionData)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  bsgo::LootMessage message(bsgo::Uuid{2}, bsgo::Uuid{4}, 12);
  message.setClientId(net::ClientId{118});
  std::stringstream out;
  message.serialize(out);
  const auto serialized = out.str();
  std::vector<char> data(serialized.begin(), serialized.end());

  net::DataReceivedEvent event(net::ClientId{17}, std::vector<char>(data.begin(), data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(bsgo::MessageType::LOOT, actual->type());
  EXPECT_EQ(net::ClientId{17}, actual->as<bsgo::LootMessage>().getClientId());
}

} // namespace bsgalone::core
