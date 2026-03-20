
#include "BroadcastMessageListener.hh"
#include "JumpMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "TestOutputNetworkAdapter.hh"
#include "TestPlayerCommand.hh"
#include "TestPlayerMessage.hh"
#include "TestSystemMessage.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener, ThrowsWhenClientManagerIsNull)
{
  EXPECT_THROW(
    []() { BroadcastMessageListener(nullptr, std::make_shared<TestOutputNetworkAdapter>()); }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener, ThrowsWhenNetworkAdapaterIsNull)
{
  EXPECT_THROW([]() { BroadcastMessageListener(std::make_shared<ClientManager>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     RegistersPlayerWhenLoginMessageIsReceived)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  BroadcastMessageListener listener(manager, std::make_shared<TestOutputNetworkAdapter>());

  EXPECT_FALSE(manager->tryGetPlayerForClient(net::ClientId{12}).has_value());
  EXPECT_FALSE(manager->tryGetSystemForClient(net::ClientId{12}).has_value());

  core::LoginMessage message(net::ClientId{12});
  message.setPlayerDbId(core::Uuid{18});
  message.setRole(core::GameRole::GUNNER);
  message.setSystemDbId(core::Uuid{19});
  listener.onEventReceived(message);

  std::cout << "hoho\n";
  EXPECT_EQ(core::Uuid{18}, manager->tryGetPlayerForClient(net::ClientId{12}).value());
  std::cout << "hehe\n";
  EXPECT_EQ(core::Uuid{19}, manager->tryGetSystemForClient(net::ClientId{12}).value());
  std::cout << "hihi\n";
  const auto clients = manager->getAllClientsForSystem(core::Uuid{19});
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, clients);
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     DoesNotRegisterPlayerWhenLoginMessageIndicatesFailure)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  BroadcastMessageListener listener(manager, std::make_shared<TestOutputNetworkAdapter>());

  EXPECT_FALSE(manager->tryGetPlayerForClient(net::ClientId{12}).has_value());
  EXPECT_FALSE(manager->tryGetSystemForClient(net::ClientId{12}).has_value());

  core::LoginMessage message(net::ClientId{12});
  listener.onEventReceived(message);

  EXPECT_FALSE(manager->tryGetPlayerForClient(net::ClientId{12}).has_value());
  EXPECT_FALSE(manager->tryGetSystemForClient(net::ClientId{12}).has_value());
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     UpdatesPlayerSystemWhenJumpMessageIsReceived)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, core::Uuid{18}, core::Uuid{19});
  BroadcastMessageListener listener(manager, std::make_shared<TestOutputNetworkAdapter>());

  EXPECT_EQ(core::Uuid{19}, manager->tryGetSystemForClient(net::ClientId{12}).value());

  core::JumpMessage message(core::Uuid{3}, core::Uuid{18}, core::Uuid{19}, core::Uuid{20});
  listener.onEventReceived(message);

  EXPECT_EQ(core::Uuid{20}, manager->tryGetSystemForClient(net::ClientId{12}).value());
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     RemovesPlayerAssociationWhenLogoutMessageIsReceived)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, core::Uuid{18}, core::Uuid{19});
  BroadcastMessageListener listener(manager, std::make_shared<TestOutputNetworkAdapter>());

  EXPECT_EQ(core::Uuid{19}, manager->tryGetSystemForClient(net::ClientId{12}).value());

  core::LogoutMessage message(core::Uuid{18}, false);
  listener.onEventReceived(message);

  EXPECT_FALSE(manager->tryGetSystemForClient(net::ClientId{12}).has_value());
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, manager->getAllClients());
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     DoesNotRemovePlayerNorConnectionWhenLogoutMessageIsReceivedWithCloseConnectionStatus)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, core::Uuid{18}, core::Uuid{19});
  BroadcastMessageListener listener(manager, std::make_shared<TestOutputNetworkAdapter>());

  EXPECT_EQ(core::Uuid{19}, manager->tryGetSystemForClient(net::ClientId{12}).value());

  core::LogoutMessage message(core::Uuid{18}, true);
  listener.onEventReceived(message);

  EXPECT_EQ(core::Uuid{19}, manager->tryGetSystemForClient(net::ClientId{12}).value());
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     RoutesPlayerCommandToClientWhenPlayerIsRegistered)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, core::Uuid{18}, core::Uuid{19});
  manager->registerClient(net::ClientId{13});
  manager->registerPlayer(net::ClientId{13}, core::Uuid{17}, core::Uuid{19});
  auto server = std::make_shared<TestOutputNetworkAdapter>();
  BroadcastMessageListener listener(manager, server);

  auto message = std::make_unique<TestPlayerCommand>(core::Uuid{18}, core::Uuid{19});
  listener.onEventReceived(*message);

  EXPECT_EQ(1u, server->messages().size());
  const auto &actual = server->messages().at(0);
  EXPECT_EQ(net::ClientId{12}, actual.clientId);
  EXPECT_EQ(core::MessageType::DOCK, actual.data->type());
  EXPECT_EQ(core::Uuid{18}, actual.data->as<TestPlayerCommand>().getPlayerDbId());
  EXPECT_EQ(core::Uuid{19}, actual.data->as<TestPlayerCommand>().getSystemDbId());
}

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     RoutesPlayerMessageToClientWhenPlayerIsRegistered)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, core::Uuid{18}, core::Uuid{19});
  manager->registerClient(net::ClientId{13});
  manager->registerPlayer(net::ClientId{13}, core::Uuid{17}, core::Uuid{19});
  auto server = std::make_shared<TestOutputNetworkAdapter>();
  BroadcastMessageListener listener(manager, server);

  auto message = std::make_unique<TestPlayerMessage>(core::Uuid{18});
  listener.onEventReceived(*message);

  EXPECT_EQ(1u, server->messages().size());
  const auto &actual = server->messages().at(0);
  EXPECT_EQ(net::ClientId{12}, actual.clientId);
  EXPECT_EQ(core::MessageType::EQUIP, actual.data->type());
  EXPECT_EQ(core::Uuid{18}, actual.data->as<TestPlayerMessage>().getPlayerDbId());
}

namespace {
auto assertMessageMatches(const std::vector<TestOutputNetworkAdapter::MessageData> &messages,
                          const net::ClientId expectedClient,
                          const core::Uuid expectedSystemMessageId)
{
  for (const auto &message : messages)
  {
    if (message.clientId == expectedClient)
    {
      return message.data->type() == core::MessageType::DOCK
             && message.data->as<TestSystemMessage>().getSystemDbId() == expectedSystemMessageId;
    }
  }

  return false;
}
} // namespace

TEST(Unit_Bsgalone_Server_Messages_BroadcastMessageListener,
     RoutesSystemMessageToAllRegisteredPlayersInTheSystem)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, core::Uuid{18}, core::Uuid{19});
  manager->registerClient(net::ClientId{13});
  manager->registerPlayer(net::ClientId{13}, core::Uuid{17}, core::Uuid{19});
  manager->registerClient(net::ClientId{14});
  manager->registerPlayer(net::ClientId{14}, core::Uuid{16}, core::Uuid{20});
  auto server = std::make_shared<TestOutputNetworkAdapter>();
  BroadcastMessageListener listener(manager, server);

  auto message = std::make_unique<TestSystemMessage>(core::Uuid{19});
  listener.onEventReceived(*message);

  const auto &messages = server->messages();
  EXPECT_EQ(2u, messages.size());

  if (!assertMessageMatches(messages, net::ClientId{12}, core::Uuid{19}))
  {
    FAIL() << "Expected message to be sent to client " << net::str(net::ClientId{12});
  }
  if (!assertMessageMatches(messages, net::ClientId{13}, core::Uuid{19}))
  {
    FAIL() << "Expected message to be sent to client " << net::str(net::ClientId{13});
  }
}

} // namespace bsgalone::server
