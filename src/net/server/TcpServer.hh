
#pragma once

#include "Connection.hh"
#include "Context.hh"
#include "ServerConfig.hh"
#include <asio/asio.hpp>
#include <core_utils/CoreObject.hh>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace net {

class TcpServer : public utils::CoreObject, public std::enable_shared_from_this<TcpServer>
{
  public:
  TcpServer(Context &context, const int port, const ServerConfig &config);
  ~TcpServer() override = default;

  auto port() const -> int;
  void start();

  private:
  int m_port;
  asio::ip::tcp::acceptor m_asioAcceptor;

  std::optional<DisconnectHandler> m_disconnectHandler{};
  std::optional<ConnectionReadyHandler> m_connectionReadyHandler{};
  std::optional<DataReceivedHandler> m_connectionDataHandler{};

  std::mutex m_connectionsLocker{};
  std::unordered_map<ConnectionId, ConnectionShPtr> m_connections{};

  void initializeFromConfig(const ServerConfig &config);
  void registerToAsio();

  void onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket);
  bool setupConnection(ConnectionShPtr connection);
};

using TcpServerShPtr = std::shared_ptr<TcpServer>;

} // namespace net
