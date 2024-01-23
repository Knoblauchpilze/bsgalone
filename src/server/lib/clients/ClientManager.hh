
#pragma once

#include "Connection.hh"
#include "Uuid.hh"
#include <atomic>
#include <core_utils/CoreObject.hh>
#include <mutex>
#include <unordered_map>

namespace bsgo {

class ClientManager : public utils::CoreObject
{
  public:
  ClientManager();
  ~ClientManager() override = default;

  auto registerConnection(const net::ConnectionId connectionId) -> Uuid;
  void removeConnection(const net::ConnectionId connectionId);

  private:
  static Uuid NEXT_CLIENT_ID;

  std::mutex m_locker{};
  std::unordered_map<net::ConnectionId, Uuid> m_clients{};
};

} // namespace bsgo
