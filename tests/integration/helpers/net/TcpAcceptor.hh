
#pragma once

#include "SocketShPtr.hh"
#include <asio.hpp>
#include <memory>

namespace test {

class TcpAcceptor : public std::enable_shared_from_this<TcpAcceptor>
{
  public:
  TcpAcceptor(asio::io_context &context, const int port);
  ~TcpAcceptor() = default;

  void registerAccept();
  auto waitForServerSocket() -> net::SocketShPtr;

  private:
  asio::ip::tcp::acceptor m_acceptor;

  std::mutex m_locker{};
  std::condition_variable m_notifier{};

  net::SocketShPtr m_socket{};

  void onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket);
};

using TcpAcceptorShPtr = std::shared_ptr<TcpAcceptor>;

} // namespace test
