#pragma once

#include "SocketShPtr.hh"
#include <atomic>
#include <memory>

namespace test {

class TestTcpServer : public std::enable_shared_from_this<TestTcpServer>
{
  public:
  TestTcpServer();
  ~TestTcpServer();

  auto port() const -> int;
  void start();
  void stop();

  /// @brief - Attempts to connect to the server and returns the **client**
  /// socket when successful. The **server** socket will be available through
  /// the `socket` method. The index should be tracked by the user and is not
  /// provided as return of this function.
  /// Calling this function concurrently will cause undefined behavior.
  /// @return - the client socket connected to the server. The socket is in
  /// a healthy state when returned.
  auto connect() -> net::SocketShPtr;

  auto socket(const std::size_t index) -> net::SocketShPtr;

  private:
  static std::atomic_int NEXT_PORT;

  int m_port{};
  asio::io_context m_context{};
  asio::ip::tcp::acceptor m_acceptor;

  std::atomic_bool m_running{false};
  std::thread m_contextThread{};

  std::vector<net::SocketShPtr> m_sockets{};

  /// @brief - This promise is used in the `connect` method to register that
  /// there is a pending connection. When the `onConnectionRequest` callback
  /// is called the promise will be fulfilled, unblocking the `connect`.
  std::promise<bool> m_pendingConnection{};

  void registerAccept();
  void onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket);
};

using TestTcpServerShPtr = std::shared_ptr<TestTcpServer>;

} // namespace test
