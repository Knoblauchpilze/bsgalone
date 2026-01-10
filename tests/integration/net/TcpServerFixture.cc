
#include "TcpServerFixture.hh"

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

auto TcpServerFixture::waitForServerSocket() -> net::details::SocketShPtr
{
  return m_acceptor->waitForServerSocket();
}

auto TcpServerFixture::getTestSockets() -> ConnectedSockets
{
  auto client = this->connectToRunningServer();
  auto server = this->waitForServerSocket();

  return ConnectedSockets{.client = std::move(client), .server = std::move(server)};
}

} // namespace test
