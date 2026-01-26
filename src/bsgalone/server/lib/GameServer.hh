

#pragma once

#include "CoreObject.hh"
#include "IEventBus.hh"
#include "INetworkServer.hh"
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace bsgalone::server {
class GameServer : public core::CoreObject
{
  public:
  GameServer(net::IEventBusShPtr eventBus);
  ~GameServer() override = default;

  /// @brief - Starts the game server on the specified port. This function will synchronously start
  /// the server and block until the game loop terminates. This can happen in one of two ways:
  ///   - the game loop crashes
  ///   - the game loop terminates successfully
  /// When the `requestStop` is called, the server will attempt to gracefully shutdown. This method
  /// will return when all resources have been released.
  /// @param port - the port to listen to
  void run(const int port);

  /// @brief - Requests a termination of the game loop. Calling this method will unblock any thread
  /// blocked on a call to the `run` function. Note that the unblocking will happen only when all
  /// resources have been released.
  void requestStop();

  private:
  std::mutex m_locker{};
  bool m_running{false};
  std::condition_variable m_notifier{};

  net::INetworkServerPtr m_tcpServer{};

  void updateRunningStatus(const bool status);
  void startSubSystems(const int port);
  void stopSubSystems();
  void waitForStopCommand();
};

} // namespace bsgalone::server
