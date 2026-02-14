
#include "BroadcastMessageListener.hh"
#include "AbstractPlayerMessage.hh"
#include "AbstractSystemMessage.hh"
#include "JumpMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "SerializationUtils.hh"
#include "TestNetworkServer.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener, ThrowsWhenClientManagerIsNull)
{
  EXPECT_THROW([]() { BroadcastMessageListener(nullptr, std::make_shared<TestNetworkServer>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener, ThrowsWhenServerIsNull)
{
  EXPECT_THROW([]() { BroadcastMessageListener(std::make_shared<ClientManager>(), nullptr); }(),
               std::invalid_argument);
}

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
    ThrowsMessage<::core::CoreException>(
      "Failed to process login message without client identifier"));
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
    ThrowsMessage<::core::CoreException>(
      "Failed to process login message without player identifier"));
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
    ThrowsMessage<::core::CoreException>(
      "Failed to process login message without system identifier"));
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

namespace {
class TestPlayerMessage : public core::AbstractPlayerMessage
{
  public:
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractPlayerMessage`.
  TestPlayerMessage(const bsgo::Uuid playerDbId)
    : core::AbstractPlayerMessage(bsgo::MessageType::DOCK, playerDbId)
  {}

  ~TestPlayerMessage() override = default;

  auto serialize(std::ostream &out) const -> std::ostream & override
  {
    ::core::serialize(out, m_messageType);
    ::core::serialize(out, m_playerDbId);

    return out;
  }

  bool deserialize(std::istream &in) override
  {
    bool ok{true};
    ok &= ::core::deserialize(in, m_messageType);
    ok &= ::core::deserialize(in, m_playerDbId);

    return ok;
  }

  auto clone() const -> std::unique_ptr<IMessage> override
  {
    return std::make_unique<TestPlayerMessage>(m_playerDbId);
  }
};
} // namespace

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     RoutesPlayerMessageToClientWhenPlayerIsRegistered)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});
  manager->registerClient(net::ClientId{13});
  manager->registerPlayer(net::ClientId{13}, bsgo::Uuid{17}, bsgo::Uuid{19});
  auto server = std::make_shared<TestNetworkServer>();
  BroadcastMessageListener listener(manager, server);

  auto message = std::make_unique<TestPlayerMessage>(bsgo::Uuid{18});
  std::stringstream out;
  message->serialize(out);
  const auto outputStr = out.str();
  std::vector<char> expected(outputStr.begin(), outputStr.end());

  listener.onMessageReceived(*message);

  EXPECT_EQ(1u, server->messages().size());
  const auto &actual = server->messages().at(0);
  EXPECT_EQ(net::ClientId{12}, actual.clientId);
  EXPECT_EQ(expected, actual.data);
}

namespace {
class TestSystemMessage : public core::AbstractSystemMessage
{
  public:
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractSystemMessage`.
  TestSystemMessage(const bsgo::Uuid systemDbId)
    : core::AbstractSystemMessage(bsgo::MessageType::DOCK, systemDbId)
  {}

  ~TestSystemMessage() override = default;

  auto serialize(std::ostream &out) const -> std::ostream & override
  {
    ::core::serialize(out, m_messageType);
    ::core::serialize(out, m_systemDbId);

    return out;
  }

  bool deserialize(std::istream &in) override
  {
    bool ok{true};
    ok &= ::core::deserialize(in, m_messageType);
    ok &= ::core::deserialize(in, m_systemDbId);

    return ok;
  }

  auto clone() const -> std::unique_ptr<IMessage> override
  {
    return std::make_unique<TestSystemMessage>(m_systemDbId);
  }
};
} // namespace

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     RoutesSystemMessageToAllRegisteredPlayersInTheSystem)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});
  manager->registerClient(net::ClientId{13});
  manager->registerPlayer(net::ClientId{13}, bsgo::Uuid{17}, bsgo::Uuid{19});
  manager->registerClient(net::ClientId{14});
  manager->registerPlayer(net::ClientId{14}, bsgo::Uuid{16}, bsgo::Uuid{20});
  auto server = std::make_shared<TestNetworkServer>();
  BroadcastMessageListener listener(manager, server);

  auto message = std::make_unique<TestSystemMessage>(bsgo::Uuid{19});
  std::stringstream out;
  message->serialize(out);
  const auto outputStr = out.str();
  std::vector<char> expected(outputStr.begin(), outputStr.end());

  listener.onMessageReceived(*message);

  EXPECT_EQ(2u, server->messages().size());
  EXPECT_THAT(server->messages(),
              Contains(
                TestNetworkServer::MessageData{.clientId = net::ClientId{12}, .data = expected}));
  EXPECT_THAT(server->messages(),
              Contains(
                TestNetworkServer::MessageData{.clientId = net::ClientId{13}, .data = expected}));
}

} // namespace bsgalone::server
