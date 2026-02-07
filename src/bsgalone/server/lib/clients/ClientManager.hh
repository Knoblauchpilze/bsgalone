
#pragma once

#include "ClientId.hh"
#include "CoreObject.hh"
#include "Uuid.hh"
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace bsgalone::server {

class ClientManager : public core::CoreObject
{
  public:
  ClientManager();
  ~ClientManager() override = default;

  void registerClient(const net::ClientId clientId);
  void registerPlayer(const net::ClientId clientId,
                      const bsgo::Uuid playerDbId,
                      const bsgo::Uuid playerSystemDbId);
  void removePlayer(const bsgo::Uuid playerDbId);
  void removePlayerConnection(const bsgo::Uuid playerDbId);
  void removeClient(const net::ClientId clientId);

  auto tryGetPlayerForClient(const net::ClientId clientId) -> std::optional<bsgo::Uuid>;
  auto getClientIdForPlayer(const bsgo::Uuid playerDbId) const -> net::ClientId;

  auto getAllClients() const -> std::vector<net::ClientId>;
  auto getAllClientsForSystem(const bsgo::Uuid systemDbId) const -> std::vector<net::ClientId>;
  auto tryGetSystemForClient(const net::ClientId clientId) const -> std::optional<bsgo::Uuid>;

  void updateSystemForPlayer(const bsgo::Uuid playerDbId, const bsgo::Uuid systemDbId);

  private:
  struct ClientData
  {
    std::optional<bsgo::Uuid> playerDbId{};
    std::optional<bsgo::Uuid> playerSystemDbId{};
  };

  mutable std::mutex m_locker{};
  std::unordered_map<net::ClientId, ClientData> m_clients{};
  std::unordered_map<bsgo::Uuid, net::ClientId> m_playerToClient{};
};

using ClientManagerShPtr = std::shared_ptr<ClientManager>;

} // namespace bsgalone::server
