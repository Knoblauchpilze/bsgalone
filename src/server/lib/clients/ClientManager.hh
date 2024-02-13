
#pragma once

#include "Connection.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace bsgo {

class ClientManager : public utils::CoreObject
{
  public:
  ClientManager();
  ~ClientManager() override = default;

  auto registerConnection(const net::ConnectionShPtr connection) -> Uuid;
  void registerPlayer(const Uuid clientId, const Uuid playerDbId, const Uuid playerSystemDbId);
  void removePlayer(const Uuid playerDbId);
  void removePlayerConnection(const Uuid playerDbId);
  void removeConnection(const net::ConnectionId connectionId);

  auto getClientIdForPlayer(const Uuid playerDbId) const -> Uuid;
  auto getConnectionForClient(const Uuid clientId) const -> net::ConnectionShPtr;
  auto getAllConnections() const -> std::vector<net::ConnectionShPtr>;
  auto getAllConnectionsForSystem(const Uuid systemDbId) const -> std::vector<net::ConnectionShPtr>;
  auto tryGetSystemForClient(const Uuid clientId) const -> std::optional<Uuid>;

  void updateSystemForPlayer(const Uuid playerDbId, const Uuid systemDbId);

  struct ConnectionData
  {
    Uuid clientId{};
    std::optional<Uuid> playerDbId{};
  };
  auto tryGetDataForConnection(const net::ConnectionId connectionId) -> ConnectionData;
  bool isStillConnected(const net::ConnectionId connectionId) const;

  private:
  static Uuid NEXT_CLIENT_ID;

  struct ClientData
  {
    Uuid clientId{};
    std::optional<Uuid> playerDbId{};
    std::optional<Uuid> playerSystemDbId{};
    net::ConnectionShPtr connection{};
  };

  mutable std::mutex m_locker{};
  std::unordered_map<Uuid, ClientData> m_clients{};
  std::unordered_map<net::ConnectionId, Uuid> m_connectionToClient{};
  std::unordered_map<Uuid, Uuid> m_playerToClient{};

  auto tryGetClientDataForConnection(const net::ConnectionId connectionId) const
    -> std::optional<ClientData>;
};

using ClientManagerShPtr = std::shared_ptr<ClientManager>;

} // namespace bsgo
