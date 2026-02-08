
#pragma once

#include "BroadcastMessageModule.hh"
#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IMessageListener.hh"
#include "INetworkServer.hh"
#include "JumpMessage.hh"
#include "LoginMessage.hh"
#include <memory>

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
  BroadcastMessageModule m_broadcastModule;

  void forwardMessageToClientManager(const bsgo::IMessage &message);

  void registerPlayer(const bsgo::LoginMessage &message);
  void updatePlayerSystem(const bsgo::JumpMessage &message);
};

using BroadcastMessageListenerPtr = std::unique_ptr<BroadcastMessageListener>;

} // namespace bsgalone::server
