
#include "ClientConnection.hh"
#include "NetworkMessageQueue.hh"
#include "SynchronizedMessageQueue.hh"

namespace pge {
constexpr auto DEFAULT_SERVER_URL = "127.0.0.1";

ClientConnection::ClientConnection(const int port, net::Context &networkContext)
  : core::CoreObject("connection")
  , m_connection(networkContext.createConnection(DEFAULT_SERVER_URL, port))
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

auto ClientConnection::createInputMessageQueue() -> bsgo::IMessageQueuePtr
{
  auto synchronizedQueue = std::make_unique<bsgo::SynchronizedMessageQueue>(
    "synchronized-message-queue-for-network");
  auto queue = std::make_unique<bsgo::NetworkMessageQueue>(std::move(synchronizedQueue));
  queue->registerToConnection(*m_connection);
  return queue;
}

} // namespace pge
