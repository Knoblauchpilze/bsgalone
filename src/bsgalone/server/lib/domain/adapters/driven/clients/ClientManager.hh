
#pragma once

#include "ClientId.hh"
#include "CoreObject.hh"
#include "ForManagingClient.hh"
#include "Uuid.hh"
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace bsgalone::server {

class ClientManager : public ForManagingClient, public ::core::CoreObject
{
  public:
  ClientManager();
  ~ClientManager() override = default;

  void registerClient(const net::ClientId clientId);
  void registerPlayer(const net::ClientId clientId,
                      const core::Uuid playerDbId,
                      const core::Uuid playerSystemDbId) override;
  void removePlayer(const core::Uuid playerDbId) override;
  void removePlayerConnection(const core::Uuid playerDbId);
  void removeClient(const net::ClientId clientId);

  auto tryGetPlayerForClient(const net::ClientId clientId) -> std::optional<core::Uuid>;
  auto getClientIdForPlayer(const core::Uuid playerDbId) const -> net::ClientId override;

  auto getAllClients() const -> std::vector<net::ClientId> override;
  auto getAllClientsForSystem(const core::Uuid systemDbId) const -> std::vector<net::ClientId>;
  auto tryGetSystemForClient(const net::ClientId clientId) const -> std::optional<core::Uuid>;

  void updateSystemForPlayer(const core::Uuid playerDbId, const core::Uuid systemDbId);

  private:
  struct ClientData
  {
    std::optional<core::Uuid> playerDbId{};
    std::optional<core::Uuid> playerSystemDbId{};
  };

  mutable std::mutex m_locker{};
  std::unordered_map<net::ClientId, ClientData> m_clients{};
  std::unordered_map<core::Uuid, net::ClientId> m_playerToClient{};
};

using ClientManagerShPtr = std::shared_ptr<ClientManager>;

} // namespace bsgalone::server
