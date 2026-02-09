
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
    [&manager]() { manager.registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19}); }(),
    core::CoreException);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, RegisteredPlayerCanBeQueried)
{
  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});

  const auto clientId = manager.getClientIdForPlayer(bsgo::Uuid{18});
  EXPECT_EQ(net::ClientId{12}, clientId);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ThrowsWhenFetchingClientForUnknownPlayer)
{
  ClientManager manager;

  EXPECT_THROW([&manager]() { manager.getClientIdForPlayer(net::ClientId{12}); }(),
               core::CoreException);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ReturnsClientForRegisteredPlayer)
{
  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});

  const auto maybePlayer = manager.tryGetPlayerForClient(net::ClientId{12});
  EXPECT_EQ(bsgo::Uuid{18}, maybePlayer.value());
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ReturnsSystemForRegisteredPlayer)
{
  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});

  const auto maybeSystem = manager.tryGetSystemForClient(net::ClientId{12});
  EXPECT_EQ(bsgo::Uuid{19}, maybeSystem.value());
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, DoesNotReturnPlayerWhenClientHasNoPlayer)
{
  ClientManager manager;
  manager.registerClient(net::ClientId{12});

  const auto clients = manager.getAllClientsForSystem(bsgo::Uuid{19});
  EXPECT_EQ(std::vector<net::ClientId>{}, clients);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, DoesNotReturnPlayerWhenNotRegisteredInSystem)
{
  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});

  const auto clients = manager.getAllClientsForSystem(bsgo::Uuid{20});
  EXPECT_EQ(std::vector<net::ClientId>{}, clients);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ReturnsClientWhenRegisteredInSystem)
{
  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});

  const auto clients = manager.getAllClientsForSystem(bsgo::Uuid{19});
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, clients);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ThrowsWhenRemovingUnknownClient)
{
  ClientManager manager;

  EXPECT_THROW([&manager]() { manager.removeClient(net::ClientId{12}); }(), core::CoreException);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, RemovedClientCannotBeQueriedAnymore)
{
  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});

  manager.removeClient(net::ClientId{12});

  EXPECT_FALSE(manager.tryGetPlayerForClient(net::ClientId{12}).has_value());
  EXPECT_FALSE(manager.tryGetSystemForClient(net::ClientId{12}).has_value());

  const auto allClients = manager.getAllClients();
  EXPECT_EQ(std::vector<net::ClientId>{}, allClients);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ThrowsWhenUpdatingSystemForUnknownPlayer)
{
  ClientManager manager;

  EXPECT_THROW([&manager]() { manager.updateSystemForPlayer(bsgo::Uuid{18}, bsgo::Uuid{20}); }(),
               core::CoreException);
}

TEST(Unit_Bsgalone_Server_Clients_ClientManager, SuccessfullyUpdatesPlayerSystem)
{
  ClientManager manager;
  manager.registerClient(net::ClientId{12});
  manager.registerPlayer(net::ClientId{12}, bsgo::Uuid{18}, bsgo::Uuid{19});

  auto clients = manager.getAllClientsForSystem(bsgo::Uuid{19});
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, clients);
  clients = manager.getAllClientsForSystem(bsgo::Uuid{20});
  EXPECT_EQ(std::vector<net::ClientId>(), clients);

  manager.updateSystemForPlayer(bsgo::Uuid{18}, bsgo::Uuid{20});

  clients = manager.getAllClientsForSystem(bsgo::Uuid{19});
  EXPECT_EQ(std::vector<net::ClientId>(), clients);
  clients = manager.getAllClientsForSystem(bsgo::Uuid{20});
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, clients);
}

} // namespace bsgalone::server
