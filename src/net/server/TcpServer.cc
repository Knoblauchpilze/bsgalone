
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
  m_acceptor              = config.acceptor;
  m_disconnectHandler     = config.disconnectHandler;
  m_connectionDataHandler = config.connectionDataHandler;
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

  auto connection = std::make_unique<Connection>(std::move(socket));
  if (m_acceptor && !(*m_acceptor)(*connection))
  {
    warn("Refused connection from " + connection->str());
    return;
  }

  info("Approved connection from " + connection->str());
  if (m_connectionDataHandler)
  {
    connection->setDataHandler(*m_connectionDataHandler);
  }
  connection->activate();
  m_connections.emplace_back(std::move(connection));
}

} // namespace net
