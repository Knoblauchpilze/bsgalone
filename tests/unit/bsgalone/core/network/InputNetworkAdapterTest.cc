
#include "InputNetworkAdapter.hh"
#include "DataReceivedEvent.hh"
#include "LootMessage.hh"
#include "MessageParser.hh"
#include "ScannedMessage.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
auto generateCompleteScannedMessage() -> std::vector<char>
{
  bsgo::ScannedMessage message(bsgo::Uuid{2}, bsgo::Uuid{4});
  std::stringstream out;
  message.serialize(out);
  const auto serialized = out.str();
  return std::vector<char>(serialized.begin(), serialized.end());
}
} // namespace

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { InputNetworkAdapter(nullptr, std::make_unique<bsgo::MessageParser>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter, ThrowsWhenMessageParserIsNull)
{
  EXPECT_THROW([]() { InputNetworkAdapter(std::make_shared<TestMessageQueue>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter, CorrectlyPublishesCompleteMessage)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

  const auto data = generateCompleteScannedMessage();
  const net::DataReceivedEvent event(net::ClientId{1}, data);

  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &message = queue->messages().at(0);
  EXPECT_EQ(MessageType::SCANNED, message->type());
  EXPECT_EQ(net::ClientId{1}, message->as<bsgo::ScannedMessage>().getClientId());
  EXPECT_EQ(bsgo::Uuid{2}, message->as<bsgo::ScannedMessage>().getPlayerDbId());
  EXPECT_EQ(bsgo::Uuid{4}, message->as<bsgo::ScannedMessage>().getAsteroidDbId());
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter, CorrectlyPublishesCompleteMessageWithNoClientId)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

  const auto data = generateCompleteScannedMessage();
  const net::DataReceivedEvent event({}, data);

  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &message = queue->messages().at(0);
  EXPECT_EQ(MessageType::SCANNED, message->type());
  EXPECT_FALSE(message->as<bsgo::ScannedMessage>().tryGetClientId().has_value());
  EXPECT_EQ(bsgo::Uuid{2}, message->as<bsgo::ScannedMessage>().getPlayerDbId());
  EXPECT_EQ(bsgo::Uuid{4}, message->as<bsgo::ScannedMessage>().getAsteroidDbId());
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter,
     DoesNotPublishMessageWhenIncompleteDataIsReceived)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());
  const net::DataReceivedEvent event(net::ClientId{0},
                                     std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter, PublishesMessageWhenAllDataIsReceived)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent(net::ClientId{0}, std::vector<char>(data.begin() + 5, data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &message = queue->messages().at(0);
  EXPECT_EQ(MessageType::SCANNED, message->type());
  EXPECT_EQ(bsgo::Uuid{2}, message->as<bsgo::ScannedMessage>().getPlayerDbId());
  EXPECT_EQ(bsgo::Uuid{4}, message->as<bsgo::ScannedMessage>().getAsteroidDbId());
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter, DoesNotPublishMessageTwice)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

  const auto data = generateCompleteScannedMessage();
  net::DataReceivedEvent event(net::ClientId{0}, data);

  adapter.onEventReceived(event);
  EXPECT_FALSE(queue->empty());

  queue->clearMessages();

  event = net::DataReceivedEvent(net::ClientId{0}, std::vector<char>());
  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter,
     DoesNotPublishMessageWhenDataReceivedForAnotherClient)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

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

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter,
     DoesNotPublishMessageWhenDataReceivedForNoClient)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent({}, std::vector<char>(data.begin(), data.begin() + 5));
  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter, OnlyPublishMessageForReceivedData)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

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
  EXPECT_EQ(MessageType::SCANNED, message->type());
  EXPECT_EQ(bsgo::Uuid{2}, message->as<bsgo::ScannedMessage>().getPlayerDbId());
  EXPECT_EQ(bsgo::Uuid{4}, message->as<bsgo::ScannedMessage>().getAsteroidDbId());
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter, ThrowsWhenReceivingInvalidData)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

  const auto data = generateCompleteScannedMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin() + 4, data.end()));

  auto body = [&adapter, &event]() { adapter.onEventReceived(event); };
  EXPECT_THROW(body(), ::core::CoreException);
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter, SetsClientIdForNetworkMessageWhenNotDefined)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

  bsgo::LootMessage message(bsgo::Uuid{2}, bsgo::Uuid{4}, 12);
  std::stringstream out;
  message.serialize(out);
  const auto serialized = out.str();
  std::vector<char> data(serialized.begin(), serialized.end());

  net::DataReceivedEvent event(net::ClientId{17}, std::vector<char>(data.begin(), data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(MessageType::LOOT, actual->type());
  EXPECT_EQ(net::ClientId{17}, actual->as<bsgo::LootMessage>().getClientId());
}

TEST(Unit_Bsgalone_Core_Network_InputNetworkAdapter, OverridesExistingClientIdWithConnectionData)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<bsgo::MessageParser>());

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
  EXPECT_EQ(MessageType::LOOT, actual->type());
  EXPECT_EQ(net::ClientId{17}, actual->as<bsgo::LootMessage>().getClientId());
}

} // namespace bsgalone::core
