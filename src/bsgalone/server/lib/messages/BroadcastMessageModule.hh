#pragma once

#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IMessage.hh"
#include "IOutputNetworkAdapter.hh"
#include "Uuid.hh"
#include <vector>

namespace bsgalone::server {

class BroadcastMessageModule : public ::core::CoreObject
{
  public:
  BroadcastMessageModule(ClientManagerShPtr clientManager, core::IOutputNetworkAdapterShPtr adapter);
  ~BroadcastMessageModule() override = default;

  void processMessage(const core::IMessage &message);

  private:
  ClientManagerShPtr m_clientManager{};
  core::IOutputNetworkAdapterShPtr m_networkAdapter{};

  void sendMessageToClient(const net::ClientId clientId, const core::IMessage &message);
  void broadcastMessage(const core::IMessage &message);

  auto tryDetermineClientId(const core::IMessage &message) const -> std::optional<bsgo::Uuid>;
  auto tryDetermineSystemIds(const core::IMessage &message) const -> std::vector<bsgo::Uuid>;
};

} // namespace bsgalone::server