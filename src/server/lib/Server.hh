

#pragma once

#include "ClientManager.hh"
#include "CoreObject.hh"
#include "MessageExchanger.hh"
#include "ServerNetworkClient.hh"
#include "SystemProcessor.hh"
#include "SystemQueues.hh"
#include <atomic>
#include <condition_variable>
#include <unordered_map>

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

  ServerNetworkClientShPtr m_networkClient{};

  ClientManagerShPtr m_clientManager{std::make_shared<ClientManager>()};
  MessageExchangerPtr m_messageExchanger{};

  std::unordered_map<Uuid, IMessageQueueShPtr> m_inputQueues{};
  std::vector<SystemProcessorShPtr> m_systemProcessors{};

  void initialize();
  void initializeSystems();
  void initializeMessageSystem();

  void setup(const int port);
  void activeRunLoop();
  void shutdown();
};

} // namespace bsgo
