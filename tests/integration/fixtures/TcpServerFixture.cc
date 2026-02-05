
#include "TcpServerFixture.hh"
#include "ClientConnectedEvent.hh"

namespace test {

void TcpServerFixture::SetUp()
{
  this->TcpFixture::SetUp();

  m_acceptor = std::make_shared<TcpAcceptor>(m_context->get(), m_port);
  m_acceptor->registerAccept();
}

void TcpServerFixture::TearDown()
{
  this->TcpFixture::TearDown();
}

auto TcpServerFixture::waitForServerSocket() -> ConnectedSockets
{
  return ConnectedSockets{.server = m_acceptor->waitForServerSocket()};
}

auto TcpServerFixture::getTestSockets() -> ConnectedSockets
{
  auto client    = this->connectToRunningServer();
  auto sockets   = this->waitForServerSocket();
  sockets.client = std::move(client);
  return sockets;
}

auto TcpServerFixture::waitForConnectionEstablishedEvent(TestEventBusShPtr &eventBus)
  -> ConnectedSockets
{
  const auto sockets = this->waitForServerSocket();

  const auto event = eventBus->waitForEvent();
  if (event->type() != net::EventType::CONNECTION_ESTABLISHED)
  {
    throw std::runtime_error("Received unexpected event " + net::str(event->type())
                             + " while waiting for connection established event");
  }

  return sockets;
}

} // namespace test
