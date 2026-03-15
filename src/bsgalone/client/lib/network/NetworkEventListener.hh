
#pragma once

#include "ConnectionEstablishedEvent.hh"
#include "ConnectionLostEvent.hh"
#include "IMessageQueue.hh"
#include "INetworkEventListener.hh"
#include <atomic>

namespace bsgalone::client {

class NetworkEventListener : public net::INetworkEventListener
{
  public:
  NetworkEventListener(std::atomic_bool &connected, core::IMessageQueueShPtr inputQueue);
  ~NetworkEventListener() override = default;

  bool isEventRelevant(const net::NetworkEventType &type) const override;
  void onEventReceived(const net::INetworkEvent &event) override;

  private:
  std::atomic_bool &m_connected;
  core::IMessageQueueShPtr m_inputQueue{};

  void handleConnectionEstablished(const net::ConnectionEstablishedEvent &event);
  void handleConnectionLost(const net::ConnectionLostEvent &event);
};

} // namespace bsgalone::client
