
#pragma once

#include "Context.hh"
#include <asio/asio.hpp>
#include <core_utils/CoreObject.hh>

namespace net {

class TcpServer : public utils::CoreObject
{
  public:
  TcpServer(Context &context, const int port);
  ~TcpServer() override = default;

  auto port() const -> int;

  private:
  int m_port;
  asio::ip::tcp::acceptor m_asioAcceptor;

  void registerToAsio();

  void onConnection(const std::error_code &code, asio::ip::tcp::socket socket);
};

} // namespace net
