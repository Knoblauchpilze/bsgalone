
#include "TcpServer.hh"
#include "AsioUtils.hh"

namespace net {

TcpServer::TcpServer(Context &context, const int port)
  : utils::CoreObject("tcp")
  , m_port(port)
  , m_asioAcceptor(context.get(), asio::ip::tcp::endpoint(asio::ip::tcp::v4(), m_port))
{
  setService("net");
  addModule("server");

  registerToAsio();
}

auto TcpServer::port() const -> int
{
  return m_port;
}

void TcpServer::registerToAsio()
{
  // https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/ReadHandler.html
  m_asioAcceptor.async_accept(
    std::bind(&TcpServer::onConnection, this, std::placeholders::_1, std::placeholders::_2));
}

void TcpServer::onConnection(const std::error_code &code, asio::ip::tcp::socket socket)
{
  if (code)
  {
    warn("Received error when processing incoming connection", code.message());
    return;
  }

  debug("Processing new connection from " + str(socket.remote_endpoint()));

  registerToAsio();

  // // Create a new connection to handle this client
  // std::shared_ptr<connection<T>> newconn
  //   = std::make_shared<connection<T>>(connection<T>::owner::server,
  //                                     m_asioContext,
  //                                     std::move(socket),
  //                                     m_qMessagesIn);

  // // Give the user server a chance to deny connection
  // if (OnClientConnect(newconn))
  // {
  //   // Connection allowed, so add to container of new connections
  //   m_deqConnections.push_back(std::move(newconn));

  //   // And very important! Issue a task to the connection's
  //   // asio context to sit and wait for bytes to arrive!
  //   m_deqConnections.back()->ConnectToClient(nIDCounter++);

  //   std::cout << "[" << m_deqConnections.back()->GetID() << "] Connection Approved\n";
  // }
  // else
  // {
  //   std::cout << "[-----] Connection Denied\n";

  //   // Connection will go out of scope with no pending tasks, so will
  //   // get destroyed automagically due to the wonder of smart pointers
  // }
}

} // namespace net
