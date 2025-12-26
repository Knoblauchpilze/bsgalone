
#pragma once

#include "SocketPtr.hh"
#include <asio.hpp>
#include <memory>

namespace test {

class TestTcpServer;
using TestTcpServerShPtr = std::shared_ptr<TestTcpServer>;

class TestTcpServer : public std::enable_shared_from_this<TestTcpServer>
{
  public:
  TestTcpServer(const int port);
  ~TestTcpServer();

  void registerAccept();

  void start();
  auto connect() -> net::SocketShPtr;
  auto socket(const std::size_t index) -> net::SocketShPtr;

  static auto create(const int port) -> TestTcpServerShPtr;

  private:
  int m_port{};
  asio::io_context m_context{};
  asio::ip::tcp::acceptor m_acceptor;

  std::atomic_bool m_running{false};
  std::thread m_contextThread{};

  std::vector<net::SocketShPtr> m_sockets{};

  void stop();
  void onConnectionRequest(const std::error_code code, asio::ip::tcp::socket socket);
};

} // namespace test
