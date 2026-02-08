
#pragma once

#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IMessageListener.hh"
#include "INetworkServer.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgalone::server {

class BroadcastMessageListener : public bsgo::IMessageListener, public core::CoreObject
{
  public:
  BroadcastMessageListener(ClientManagerShPtr clientManager, net::INetworkServerShPtr server);
  ~BroadcastMessageListener() override = default;

  bool isMessageRelevant(const bsgo::MessageType &type) const override;
  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  ClientManagerShPtr m_clientManager{};
  net::INetworkServerShPtr m_server{};

  void processMessage(const bsgo::IMessage &message);

  void sendMessageToClient(const net::ClientId clientId, const bsgo::IMessage &message);
  void broadcastMessage(const bsgo::IMessage &message);

  auto tryDetermineClientId(const bsgo::IMessage &message) const -> std::optional<bsgo::Uuid>;
  auto tryDetermineSystemIds(const bsgo::IMessage &message) const -> std::vector<bsgo::Uuid>;
};

using BroadcastMessageListenerPtr = std::unique_ptr<BroadcastMessageListener>;

} // namespace bsgalone::server
