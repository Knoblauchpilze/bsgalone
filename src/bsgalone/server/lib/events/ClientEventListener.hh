
#pragma once

#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "ClientManager.hh"
#include "IEventListener.hh"
#include "IMessageQueue.hh"

namespace bsgalone::server {

class ClientEventListener : public net::IEventListener
{
  public:
  ClientEventListener(ClientManagerShPtr clientManager,
                      bsgalone::core::IMessageQueueShPtr inputQueue);

  ~ClientEventListener() override = default;

  bool isEventRelevant(const net::EventType &type) const override;
  void onEventReceived(const net::IEvent &event) override;

  private:
  ClientManagerShPtr m_clientManager{};
  bsgalone::core::IMessageQueueShPtr m_inputQueue{};

  void handleClientConnected(const net::ClientConnectedEvent &event);
  void handleClientDisconnected(const net::ClientDisconnectedEvent &event);
};

} // namespace bsgalone::server
