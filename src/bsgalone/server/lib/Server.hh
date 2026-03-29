

#pragma once

#include "CoreObject.hh"
#include "IGameEventQueue.hh"
#include "ServerNetworkClient.hh"
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

  /// @brief - The network client used to accept and process client connections
  ServerNetworkClientShPtr m_networkClient{};

  /// @brief - Defines the event queue used by use cases to communicate with
  /// one another through events. This is used as:
  ///   - an output queue for use cases when they need to produce events to
  ///     send to the client applications or to other use cases
  ///   - an input queue for use cases to receive updates from other use cases
  IGameEventQueueShPtr m_eventQueue{};

  void initialize();
  void initializeMessageSystem();

  void setup(const int port);
  void activeRunLoop();
  void shutdown();
};

} // namespace bsgalone::server
