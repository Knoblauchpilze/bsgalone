
#include "OutputNetworkAdapter.hh"
#include "ScannedMessage.hh"
#include "TestNetworkClient.hh"
#include "TestNetworkServer.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
TEST(Unit_Bsgalone_Core_Network_OutputNetworkAdapter, ThrowsWhenServerIsNull)
{
  EXPECT_THROW([]() { OutputNetworkAdapter(std::shared_ptr<TestNetworkServer>{}); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Network_OutputNetworkAdapter, ThrowsWhenClientIsNull)
{
  EXPECT_THROW([]() { OutputNetworkAdapter(std::shared_ptr<TestNetworkClient>{}); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Network_OutputNetworkAdapter,
     ThrowsWhenSendingAServerMessageWithClientConfigured)
{
  OutputNetworkAdapter adapter(std::make_shared<TestNetworkClient>());

  EXPECT_THROW(
    [&adapter]() {
      bsgo::ScannedMessage message(bsgo::Uuid{2}, bsgo::Uuid{4});
      adapter.sendMessage(net::ClientId{12}, message);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Network_OutputNetworkAdapter,
     ThrowsWhenSendingAClientMessageWithServerConfigured)
{
  OutputNetworkAdapter adapter(std::make_shared<TestNetworkServer>());

  EXPECT_THROW(
    [&adapter]() {
      bsgo::ScannedMessage message(bsgo::Uuid{2}, bsgo::Uuid{4});
      adapter.sendMessage(message);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Network_OutputNetworkAdapter, CorrectlySendsServerMessage)
{
  auto server = std::make_shared<TestNetworkServer>();
  OutputNetworkAdapter adapter(server);

  bsgo::ScannedMessage message(bsgo::Uuid{2}, bsgo::Uuid{4});
  adapter.sendMessage(net::ClientId{12}, message);

  EXPECT_EQ(1u, server->messages().size());
  const auto &actual = server->messages().at(0);
  EXPECT_EQ(net::ClientId{12}, actual.clientId);

  std::stringstream out;
  message.serialize(out);
  const auto serialized = out.str();
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(expected, actual.data);
}

TEST(Unit_Bsgalone_Core_Network_OutputNetworkAdapter, CorrectlySendsClientMessage)
{
  auto client = std::make_shared<TestNetworkClient>();
  OutputNetworkAdapter adapter(client);

  bsgo::ScannedMessage message(bsgo::Uuid{2}, bsgo::Uuid{4});
  adapter.sendMessage(message);

  EXPECT_EQ(1u, client->messages().size());
  const auto &actual = client->messages().at(0);

  std::stringstream out;
  message.serialize(out);
  const auto serialized = out.str();
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(expected, actual.data);
}

} // namespace bsgalone::core
