
#include "NetworkAdapter.hh"

#include "DataReceivedEvent.hh"
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

  void processMessages(const std::optional<int> & /*amount*/) override
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

TEST(Unit_Bsgalone_Core_Network_ClientManager, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { NetworkAdapter(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Network_ClientManager, CorrectlyPublishesCompleteMessage)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  const net::DataReceivedEvent event(net::ClientId{0}, data);

  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &message = queue->messages().at(0);
  EXPECT_EQ(bsgo::MessageType::SCANNED, message->type());
  EXPECT_EQ(bsgo::Uuid{2}, message->as<bsgo::ScannedMessage>().getPlayerDbId());
  EXPECT_EQ(bsgo::Uuid{4}, message->as<bsgo::ScannedMessage>().getAsteroidDbId());
}

TEST(Unit_Bsgalone_Core_Network_ClientManager, DoesNotPublishMessageWhenIncompleteDataIsReceived)
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

TEST(Unit_Bsgalone_Core_Network_ClientManager, PublishesMessageWhenAllDataIsReceived)
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

TEST(Unit_Bsgalone_Core_Network_ClientManager, DoesNotPublishMessageTwice)
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

TEST(Unit_Bsgalone_Core_Network_ClientManager, DoesNotPublishMessageWhenDataReceivedForAnotherClient)
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

TEST(Unit_Bsgalone_Core_Network_ClientManager, OnlyPublishMessageForReceivedData)
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

TEST(Unit_Bsgalone_Core_Network_ClientManager, ThrowsWhenReceivingInvalidData)
{
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkAdapter adapter(queue);

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin() + 5, data.end()));

  auto body = [&adapter, &event]() { adapter.onEventReceived(event); };
  EXPECT_THROW(body(), ::core::CoreException);
}

} // namespace bsgalone::core
