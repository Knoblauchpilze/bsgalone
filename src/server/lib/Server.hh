

#pragma once

#include "BroadcastMessageQueue.hh"
#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include "INetworkServer.hh"
#include "MessageExchanger.hh"
#include "SystemProcessor.hh"
#include <atomic>
#include <condition_variable>

namespace bsgo {
class Server : public core::CoreObject
{
  public:
  Server();
  ~Server() override = default;

  void run(const int port);
  void requestStop();

  private:
  std::atomic_bool m_running{false};
  std::mutex m_runningLocker{};
  std::condition_variable m_runningNotifier{};

  net::IEventBusShPtr m_eventBus{};
  net::INetworkServerShPtr m_tcpServer{};

  ClientManagerShPtr m_clientManager{std::make_shared<ClientManager>()};
  MessageExchangerPtr m_messageExchanger{};
  std::vector<SystemProcessorShPtr> m_systemProcessors{};

  void initialize();
  void initializeSystems();
  void initializeMessageSystem();

  void setup(const int port);
  void activeRunLoop();
  void shutdown();
};

} // namespace bsgo
