
#include "TcpServer.hh"
#include "AsioUtils.hh"

namespace net {

TcpServer::TcpServer(Context &context, const int port, const ServerConfig &config)
  : utils::CoreObject("tcp")
  , m_port(port)
  , m_asioAcceptor(context.get(), asio::ip::tcp::endpoint(asio::ip::tcp::v4(), m_port))
{
  setService("net");
  addModule("server");

  initializeFromConfig(config);
  registerToAsio();
}

auto TcpServer::port() const -> int
{
  return m_port;
}

void TcpServer::initializeFromConfig(const ServerConfig &config)
{
  m_acceptor               = config.acceptor;
  m_disconnectHandler      = config.disconnectHandler;
  m_connectionReadyHandler = config.connectionReadyHandler;
  m_connectionDataHandler  = config.connectionDataHandler;
}

void TcpServer::registerToAsio()
{
  // https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/ReadHandler.html
  m_asioAcceptor.async_accept(
    std::bind(&TcpServer::onConnectionRequest, this, std::placeholders::_1, std::placeholders::_2));
}

void TcpServer::onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket)
{
  registerToAsio();

  if (code)
  {
    warn("Error detected when processing incoming connection", code.message());
    return;
  }

  const auto client = str(socket.remote_endpoint());
  debug("Processing new connection from " + client);

  auto connection = std::make_shared<Connection>(std::move(socket));
  if (setupConnection(connection) && m_connectionReadyHandler)
  {
    (*m_connectionReadyHandler)(*connection);
  }
}

bool TcpServer::setupConnection(ConnectionShPtr connection)
{
  if (m_acceptor && !(*m_acceptor)(*connection))
  {
    warn("Refused connection from " + connection->str());
    return false;
  }

  info("Approved connection from " + connection->str());
  if (m_disconnectHandler)
  {
    connection->setDisconnectHandler(*m_disconnectHandler);
  }
  if (m_connectionDataHandler)
  {
    connection->setDataHandler(*m_connectionDataHandler);
  }
  connection->connect();

  {
    const std::lock_guard guard(m_connectionsLocker);
    const auto id = connection->id();
    m_connections.emplace(id, std::move(connection));
  }

  return true;
}

} // namespace net
