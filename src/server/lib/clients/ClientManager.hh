
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
  void registerPlayer(const Uuid clientId, const Uuid playerDbId);
  void removePlayer(const Uuid playerDbId);
  void removeConnection(const net::ConnectionId connectionId);

  auto getConnectionForClient(const Uuid clientId) const -> net::ConnectionShPtr;
  auto getAllConnections() const -> std::vector<net::ConnectionShPtr>;

  private:
  static Uuid NEXT_CLIENT_ID;

  struct ClientData
  {
    Uuid clientId{};
    std::optional<Uuid> playerDbId{};
    net::ConnectionShPtr connection{};
  };

  mutable std::mutex m_locker{};
  std::unordered_map<Uuid, ClientData> m_clients{};
  std::unordered_map<net::ConnectionId, Uuid> m_connectionToClient{};
  std::unordered_map<Uuid, Uuid> m_playerToClient{};
};

using ClientManagerShPtr = std::shared_ptr<ClientManager>;

} // namespace bsgo
