#pragma once

#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IMessage.hh"
#include "INetworkServer.hh"
#include "Uuid.hh"
#include <vector>

namespace bsgalone::server {

class BroadcastMessageModule : public core::CoreObject
{
  public:
  BroadcastMessageModule(ClientManagerShPtr clientManager, net::INetworkServerShPtr server);
  ~BroadcastMessageModule() override = default;

  void processMessage(const bsgo::IMessage &message);

  private:
  ClientManagerShPtr m_clientManager{};
  net::INetworkServerShPtr m_server{};

  void sendMessageToClient(const net::ClientId clientId, const bsgo::IMessage &message);
  void broadcastMessage(const bsgo::IMessage &message);

  auto tryDetermineClientId(const bsgo::IMessage &message) const -> std::optional<bsgo::Uuid>;
  auto tryDetermineSystemIds(const bsgo::IMessage &message) const -> std::vector<bsgo::Uuid>;
};

} // namespace bsgalone::server