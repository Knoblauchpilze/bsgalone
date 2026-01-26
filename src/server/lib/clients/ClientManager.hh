
#pragma once

#include "Connection.hh"
#include "CoreObject.hh"
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

  void registerConnection(const net::ConnectionShPtr connection);
  void registerPlayer(const net::ClientId clientId,
                      const Uuid playerDbId,
                      const Uuid playerSystemDbId);
  void removePlayer(const Uuid playerDbId);
  void removePlayerConnection(const Uuid playerDbId);
  void markConnectionAsStale(const net::ClientId clientId);
  void removeConnection(const net::ClientId clientId);

  auto getClientIdForPlayer(const Uuid playerDbId) const -> net::ClientId;

  /// @brief - Try to retrieve the connection linked to the input client. It can
  /// fail to find one in case the connection is marked as stale in which case we
  /// don't return it as sending data would fail anyway.
  /// @param clientId - the client id for which the connection should be retrieved.
  /// @return - the connection associated to the client if it is not stale.
  auto tryGetConnectionForClient(const net::ClientId clientId) const
    -> std::optional<net::ConnectionShPtr>;

  auto getAllConnections() const -> std::vector<net::ConnectionShPtr>;
  auto getAllConnectionsForSystem(const Uuid systemDbId) const -> std::vector<net::ConnectionShPtr>;
  auto tryGetSystemForClient(const net::ClientId clientId) const -> std::optional<Uuid>;

  void updateSystemForPlayer(const Uuid playerDbId, const Uuid systemDbId);

  struct ConnectionData
  {
    std::optional<Uuid> playerDbId{};
    bool stale{false};
  };
  auto tryGetDataForConnection(const net::ClientId clientId) -> ConnectionData;
  bool isStillConnected(const net::ClientId clientId) const;

  private:
  struct ClientData
  {
    std::optional<Uuid> playerDbId{};
    std::optional<Uuid> playerSystemDbId{};
    net::ConnectionShPtr connection{};
    bool connectionIsStale{false};
  };

  mutable std::mutex m_locker{};
  std::unordered_map<net::ClientId, ClientData> m_clients{};
  std::unordered_map<Uuid, net::ClientId> m_playerToClient{};

  auto tryGetClientDataForConnection(const net::ClientId clientId) const
    -> std::optional<ClientData>;
};

using ClientManagerShPtr = std::shared_ptr<ClientManager>;

} // namespace bsgo
