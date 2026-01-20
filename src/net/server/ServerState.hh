
#pragma once

#include <mutex>

namespace net::details {

class ServerState
{
  public:
  ServerState()  = default;
  ~ServerState() = default;

  bool isRegistered();
  bool isAccepting();

  /// @brief - Attempts to mark the server state as registered. This function will return
  /// whether or not the operation could be completed.
  /// The first call to this function will succeed and any subsequent call will return false
  /// as it is only possible to register once.
  /// @return - whether or not the state is now marked as registered
  bool markAsRegistered();

  /// @brief - Attempts to mark the server state as terminating. This means that although
  /// the server is still registered, it will not accept incoming connections anymore.
  /// The first call to this function will succeed and any subsequent call will return false
  /// as it is only possible to stop the server once.
  /// @return - whether or not the state is now marked as terminating
  bool markAsTerminating();

  void registerTermination();

  private:
  std::mutex m_locker{};

  /// @brief - Controls whether the server is registered to asio. This happens in the
  /// `start` method and it prevents this method being called multiple times.
  bool m_registered{false};

  /// @brief - Controls whether the server accepts incoming connections. It is changed
  /// in the `shutdown` method and allows to gracefully shutdown the server.
  bool m_accepting{false};
};

} // namespace net::details
