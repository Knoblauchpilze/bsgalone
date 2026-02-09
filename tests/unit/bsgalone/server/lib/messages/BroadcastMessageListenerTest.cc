
#include "BroadcastMessageListener.hh"
#include "JumpMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::server {
namespace {
class TestNetworkServer : public net::INetworkServer
{
  public:
  TestNetworkServer()           = default;
  ~TestNetworkServer() override = default;

  void start(const int /*port*/) override {}

  void stop() override {}

  auto trySend(const net::ClientId /*clientId*/, std::vector<char> /*bytes*/)
    -> std::optional<net::MessageId> override
  {
    return {};
  };
};
} // namespace

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     RegistersPlayerWhenLoginMessageIsReceived)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  BroadcastMessageListener listener(manager, std::make_shared<TestNetworkServer>());

  EXPECT_FALSE(manager->tryGetPlayerForClient(net::ClientId{12}).has_value());
  EXPECT_FALSE(manager->tryGetSystemForClient(net::ClientId{12}).has_value());

  bsgo::LoginMessage message;
  message.setClientId(net::ClientId{12});
  message.setPlayerDbId(bsgo::Uuid{18});
  message.setSystemDbId(bsgo::Uuid{19});
  listener.onMessageReceived(message);

  EXPECT_EQ(bsgo::Uuid{18}, manager->tryGetPlayerForClient(net::ClientId{12}).value());
  EXPECT_EQ(bsgo::Uuid{19}, manager->tryGetSystemForClient(net::ClientId{12}).value());
  const auto clients = manager->getAllClientsForSystem(bsgo::Uuid{19});
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, clients);
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     ThrowsWhenLoginMessageDoesNotDefineClientId)
{
  auto manager = std::make_shared<ClientManager>();
  BroadcastMessageListener listener(manager, std::make_shared<TestNetworkServer>());

  EXPECT_THAT(
    [&listener]() {
      bsgo::LoginMessage message;
      message.setPlayerDbId(bsgo::Uuid{18});
      message.setSystemDbId(bsgo::Uuid{19});
      listener.onMessageReceived(message);
    },
    ThrowsMessage<core::CoreException>("Failed to process login message without client identifier"));
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     ThrowsWhenLoginMessageDoesNotDefinePlayerId)
{
  auto manager = std::make_shared<ClientManager>();
  BroadcastMessageListener listener(manager, std::make_shared<TestNetworkServer>());

  EXPECT_THAT(
    [&listener]() {
      bsgo::LoginMessage message;
      message.setClientId(net::ClientId{12});
      message.setSystemDbId(bsgo::Uuid{19});
      listener.onMessageReceived(message);
    },
    ThrowsMessage<core::CoreException>("Failed to process login message without player identifier"));
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     ThrowsWhenLoginMessageDoesNotDefineSystemId)
{
  auto manager = std::make_shared<ClientManager>();
  BroadcastMessageListener listener(manager, std::make_shared<TestNetworkServer>());

  EXPECT_THAT(
    [&listener]() {
      bsgo::LoginMessage message;
      message.setClientId(net::ClientId{12});
      message.setPlayerDbId(bsgo::Uuid{18});
      listener.onMessageReceived(message);
    },
    ThrowsMessage<core::CoreException>("Failed to process login message without system identifier"));
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     UpdatesPlayerSystemWhenJumpMessageIsReceived)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});
  BroadcastMessageListener listener(manager, std::make_shared<TestNetworkServer>());

  EXPECT_EQ(bsgo::Uuid{19}, manager->tryGetSystemForClient(net::ClientId{12}).value());

  bsgo::JumpMessage message(bsgo::Uuid{3}, bsgo::Uuid{18}, bsgo::Uuid{19}, bsgo::Uuid{20});
  listener.onMessageReceived(message);

  EXPECT_EQ(bsgo::Uuid{20}, manager->tryGetSystemForClient(net::ClientId{12}).value());
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     RemovesPlayerAssociationWhenLogoutMessageIsReceived)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});
  BroadcastMessageListener listener(manager, std::make_shared<TestNetworkServer>());

  EXPECT_EQ(bsgo::Uuid{19}, manager->tryGetSystemForClient(net::ClientId{12}).value());

  bsgo::LogoutMessage message(bsgo::Uuid{18}, false);
  listener.onMessageReceived(message);

  EXPECT_FALSE(manager->tryGetSystemForClient(net::ClientId{12}).has_value());
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, manager->getAllClients());
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     DoesNotRemovePlayerNorConnectionWhenLogoutMessageIsReceivedWithCloseConnectionStatus)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});
  BroadcastMessageListener listener(manager, std::make_shared<TestNetworkServer>());

  EXPECT_EQ(bsgo::Uuid{19}, manager->tryGetSystemForClient(net::ClientId{12}).value());

  bsgo::LogoutMessage message(bsgo::Uuid{18}, true);
  listener.onMessageReceived(message);

  EXPECT_EQ(bsgo::Uuid{19}, manager->tryGetSystemForClient(net::ClientId{12}).value());
}

} // namespace bsgalone::server
