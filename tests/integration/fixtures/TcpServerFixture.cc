
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

auto TcpServerFixture::waitForClientConnectedEvent(TestEventBusShPtr &eventBus)
  -> std::pair<net::ClientId, ConnectedSockets>
{
  const auto sockets = this->waitForServerSocket();

  const auto event = eventBus->waitForEvent();
  if (event->type() != net::EventType::CLIENT_CONNECTED)
  {
    throw std::runtime_error("Received unexpected event " + net::str(event->type())
                             + " while waiting for client connected event");
  }

  // TODO: The client id can probably not be provided anymore when the connection
  // established event is sent
  const auto actualClientId = event->as<net::ClientConnectedEvent>().clientId();
  return {actualClientId, sockets};
}

} // namespace test
