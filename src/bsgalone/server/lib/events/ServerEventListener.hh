
#pragma once

#include "INetworkEventListener.hh"
#include "ServerStartedEvent.hh"
#include "ServerStoppedEvent.hh"
#include <atomic>

namespace bsgalone::server {

class ServerEventListener : public net::INetworkEventListener
{
  public:
  ServerEventListener(std::atomic_bool &started);
  ~ServerEventListener() = default;

  bool isEventRelevant(const net::EventType &type) const override;
  void onEventReceived(const net::IEvent &event) override;

  private:
  std::atomic_bool &m_started;

  void handleServerStarted(const net::ServerStartedEvent & /*event*/);
  void handleServerStopped(const net::ServerStoppedEvent & /*event*/);
};

} // namespace bsgalone::server
