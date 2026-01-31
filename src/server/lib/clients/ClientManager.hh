
#pragma once

#include "ClientId.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include "Uuid.hh"
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace bsgo {

class ClientManager : public core::CoreObject
{
  public:
  ClientManager();
  ~ClientManager() override = default;

  void registerClient(const net::ClientId clientId);
  void registerPlayer(const net::ClientId clientId,
                      const Uuid playerDbId,
                      const Uuid playerSystemDbId);
  void removePlayer(const Uuid playerDbId);
  void removePlayerConnection(const Uuid playerDbId);
  void removeClient(const net::ClientId clientId);

  auto tryGetPlayerForClient(const net::ClientId clientId) -> std::optional<Uuid>;
  auto getClientIdForPlayer(const Uuid playerDbId) const -> net::ClientId;

  auto getAllClients() const -> std::vector<net::ClientId>;
  auto getAllClientsForSystem(const Uuid systemDbId) const -> std::vector<net::ClientId>;
  auto tryGetSystemForClient(const net::ClientId clientId) const -> std::optional<Uuid>;

  void updateSystemForPlayer(const Uuid playerDbId, const Uuid systemDbId);

  private:
  struct ClientData
  {
    std::optional<Uuid> playerDbId{};
    std::optional<Uuid> playerSystemDbId{};
  };

  mutable std::mutex m_locker{};
  std::unordered_map<net::ClientId, ClientData> m_clients{};
  std::unordered_map<Uuid, net::ClientId> m_playerToClient{};
};

using ClientManagerShPtr = std::shared_ptr<ClientManager>;

} // namespace bsgo
