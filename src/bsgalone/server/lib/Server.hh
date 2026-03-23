

#pragma once

#include "AsyncGameEventQueue.hh"
#include "CoreObject.hh"
#include "MessageExchanger.hh"
#include "ServerNetworkClient.hh"
#include "SynchronizedGameEventQueue.hh"
#include "SystemProcessor.hh"
#include "SystemQueues.hh"
#include <atomic>
#include <condition_variable>
#include <unordered_map>

namespace bsgalone::server {
class Server : public ::core::CoreObject
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

  ServerNetworkClientShPtr m_networkClient{std::make_shared<ServerNetworkClient>()};
  MessageExchangerPtr m_messageExchanger{};

  std::unordered_map<core::Uuid, core::IMessageQueueShPtr> m_inputQueues{};
  std::vector<SystemProcessorShPtr> m_systemProcessors{};

  core::IGameEventQueueShPtr m_eventQueue{};

  void initialize();
  void initializeSystems();
  void initializeMessageSystem();

  void setup(const int port);
  void activeRunLoop();
  void shutdown();
};

} // namespace bsgalone::server
