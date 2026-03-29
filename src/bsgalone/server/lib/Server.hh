

#pragma once

#include "CoreObject.hh"
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

  void initialize();

  void setup(const int port);
  void activeRunLoop();
  void shutdown();
};

} // namespace bsgalone::server
