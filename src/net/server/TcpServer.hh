
#pragma once

#include "Connection.hh"
#include "Context.hh"
#include "ServerConfig.hh"
#include <asio/asio.hpp>
#include <core_utils/CoreObject.hh>
#include <memory>
#include <optional>
#include <vector>

namespace net {

class TcpServer : public utils::CoreObject
{
  public:
  TcpServer(Context &context, const int port, const ServerConfig &config);
  ~TcpServer() override = default;

  auto port() const -> int;

  private:
  int m_port;
  asio::ip::tcp::acceptor m_asioAcceptor;

  std::optional<ConnectionAcceptor> m_acceptor{};
  std::optional<ConnectionLostHandler> m_disconnectHandler{};
  std::optional<DataReceivedHandler> m_connectionDataHandler{};

  std::vector<ConnectionShPtr> m_connections{};

  void initializeFromConfig(const ServerConfig &config);
  void registerToAsio();

  void onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket);
};

using TcpServerPtr = std::unique_ptr<TcpServer>;

} // namespace net
