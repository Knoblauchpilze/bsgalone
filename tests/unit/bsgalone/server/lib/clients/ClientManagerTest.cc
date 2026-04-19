
#include "ClientManager.hh"
#include <gtest/gtest.h>

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Clients_ClientManager, RegisteredClientIsReturnedInListOfAllClients)
{
  ClientManager manager;

  manager.registerClient(net::ClientId{12});

  const auto &allClients = manager.getAllClients();
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, allClients);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ThrowsWhenRegisteringPlayerForUnknownClient)
{
  ClientManager manager;

  EXPECT_THROW(
    [&manager]() { manager.registerPlayer(net::ClientId{12}, core::Uuid{}, core::Uuid{}); }(),
    ::core::CoreException);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, RegisteredPlayerCanBeQueried)
{
  const core::Uuid playerDbId;

  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, playerDbId, core::Uuid{});

  const auto clientId = manager.getClientIdForPlayer(playerDbId);
  EXPECT_EQ(net::ClientId{12}, clientId);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ThrowsWhenFetchingClientForUnknownPlayer)
{
  ClientManager manager;

  EXPECT_THROW([&manager]() { manager.getClientIdForPlayer(core::Uuid{}); }(),
               ::core::CoreException);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ReturnsClientForRegisteredPlayer)
{
  const core::Uuid playerDbId;

  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, playerDbId, core::Uuid{});

  const auto maybePlayer = manager.tryGetPlayerForClient(net::ClientId{12});
  EXPECT_EQ(playerDbId, maybePlayer.value());
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ReturnsSystemForRegisteredPlayer)
{
  const core::Uuid systemDbId;

  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, core::Uuid{}, systemDbId);

  const auto maybeSystem = manager.tryGetSystemForClient(net::ClientId{12});
  EXPECT_EQ(systemDbId, maybeSystem.value());
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, DoesNotReturnPlayerWhenClientHasNoPlayer)
{
  ClientManager manager;
  manager.registerClient(net::ClientId{12});

  const auto clients = manager.getAllClientsForSystem(core::Uuid{});
  EXPECT_EQ(std::vector<net::ClientId>{}, clients);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, DoesNotReturnPlayerWhenNotRegisteredInSystem)
{
  const core::Uuid systemDbId1;
  const core::Uuid systemDbId2;

  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, core::Uuid{}, systemDbId1);

  const auto clients = manager.getAllClientsForSystem(systemDbId2);
  EXPECT_EQ(std::vector<net::ClientId>{}, clients);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ReturnsClientWhenRegisteredInSystem)
{
  const core::Uuid systemDbId;

  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, core::Uuid{}, systemDbId);

  const auto clients = manager.getAllClientsForSystem(systemDbId);
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, clients);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ThrowsWhenRemovingUnknownClient)
{
  ClientManager manager;

  EXPECT_THROW([&manager]() { manager.removeClient(net::ClientId{12}); }(), ::core::CoreException);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, RemovedClientCannotBeQueriedAnymore)
{
  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, core::Uuid{}, core::Uuid{});

  manager.removeClient(net::ClientId{12});

  EXPECT_FALSE(manager.tryGetPlayerForClient(net::ClientId{12}).has_value());
  EXPECT_FALSE(manager.tryGetSystemForClient(net::ClientId{12}).has_value());

  const auto allClients = manager.getAllClients();
  EXPECT_EQ(std::vector<net::ClientId>{}, allClients);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ThrowsWhenUpdatingSystemForUnknownPlayer)
{
  ClientManager manager;

  EXPECT_THROW([&manager]() { manager.updateSystemForPlayer(core::Uuid{}, core::Uuid{}); }(),
               ::core::CoreException);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, SuccessfullyUpdatesPlayerSystem)
{
  const core::Uuid playerDbId;
  const core::Uuid systemDbId1;
  const core::Uuid systemDbId2;

  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, playerDbId, systemDbId1);

  auto clients = manager.getAllClientsForSystem(systemDbId1);
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, clients);
  clients = manager.getAllClientsForSystem(systemDbId2);
  EXPECT_EQ(std::vector<net::ClientId>(), clients);

  manager.updateSystemForPlayer(playerDbId, systemDbId2);

  clients = manager.getAllClientsForSystem(systemDbId1);
  EXPECT_EQ(std::vector<net::ClientId>(), clients);
  clients = manager.getAllClientsForSystem(systemDbId2);
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, clients);
}

} // namespace bsgalone::server
