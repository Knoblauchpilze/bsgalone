
#pragma once

#include "AbstractPlayerCommand.hh"
#include "AbstractSystemMessage.hh"
#include "BroadcastMessageModule.hh"
#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IMessageListener.hh"
#include "IOutputNetworkAdapter.hh"
#include "JumpMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include <memory>

namespace bsgalone::server {

class BroadcastMessageListener : public core::IMessageListener, public ::core::CoreObject
{
  public:
  BroadcastMessageListener(ClientManagerShPtr clientManager,
                           core::IOutputNetworkAdapterShPtr server);
  ~BroadcastMessageListener() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &message) override;

  private:
  ClientManagerShPtr m_clientManager{};
  core::IOutputNetworkAdapterShPtr m_adapter{};
  BroadcastMessageModule m_broadcastModule;

  void forwardMessageToClientManager(const core::IMessage &message);
  void triageOutboundMessage(const core::IMessage &message);
  void routePlayerCommand(const core::AbstractPlayerCommand &message);
  void routeSystemMessage(const core::AbstractSystemMessage &message);

  void registerPlayer(const core::LoginMessage &message);
  void unregisterPlayer(const core::LogoutMessage &message);
  void updatePlayerSystem(const core::JumpMessage &message);
};

using BroadcastMessageListenerPtr = std::unique_ptr<BroadcastMessageListener>;

} // namespace bsgalone::server
