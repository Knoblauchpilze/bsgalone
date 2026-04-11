
#pragma once

#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "ClientManager.hh"
#include "INetworkEventListener.hh"

namespace bsgalone::server {

class ClientEventListener : public net::INetworkEventListener
{
  public:
  explicit ClientEventListener(ClientManagerShPtr clientManager);

  ~ClientEventListener() override = default;

  bool isEventRelevant(const net::NetworkEventType &type) const override;
  void onEventReceived(const net::INetworkEvent &event) override;

  private:
  ClientManagerShPtr m_clientManager{};

  void handleClientConnected(const net::ClientConnectedEvent &event);
  void handleClientDisconnected(const net::ClientDisconnectedEvent &event);
};

} // namespace bsgalone::server
