
#include "ClientConnection.hh"
#include "NetworkMessageQueue.hh"

namespace pge {
constexpr auto DEFAULT_SERVER_URL  = "127.0.0.1";
constexpr auto DEFAULT_SERVER_PORT = 60000;

ClientConnection::ClientConnection(net::Context &networkContext)
  : utils::CoreObject("connection")
  , m_connection(networkContext.createConnection(DEFAULT_SERVER_URL, DEFAULT_SERVER_PORT))
{
  setService("network");
  m_connection->connect();
}

void ClientConnection::setDataHandler(const net::DataReceivedHandler &handler)
{
  m_connection->setDataHandler(handler);
}

void ClientConnection::sendMessage(const bsgo::IMessage &message)
{
  m_connection->send(message);
}

auto ClientConnection::connectMessageQueue(bsgo::IMessageQueuePtr messageQueue)
  -> bsgo::IMessageQueuePtr
{
  auto queue = std::make_unique<bsgo::NetworkMessageQueue>(std::move(messageQueue));
  queue->registerToConnection(*m_connection);
  return queue;
}

} // namespace pge
