
#pragma once

#include "ConnectionEstablishedEvent.hh"
#include "ConnectionLostEvent.hh"
#include "INetworkEventListener.hh"
#include "IOutputNetworkAdapter.hh"
#include <atomic>

namespace bsgalone::client {

class NetworkEventListener : public net::INetworkEventListener
{
  public:
  NetworkEventListener(std::atomic_bool &connected);
  ~NetworkEventListener() override = default;

  bool isEventRelevant(const net::NetworkEventType &type) const override;
  void onEventReceived(const net::INetworkEvent &event) override;

  private:
  std::atomic_bool &m_connected;

  void handleConnectionEstablished(const net::ConnectionEstablishedEvent &event);
  void handleConnectionLost(const net::ConnectionLostEvent &event);
};

} // namespace bsgalone::client
