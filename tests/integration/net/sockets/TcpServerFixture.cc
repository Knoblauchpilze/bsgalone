
#include "TcpServerFixture.hh"

namespace test {

void TcpServerFixture::SetUp()
{
  this->TcpFixture::SetUp();

  m_acceptor = std::make_shared<TcpAcceptor>(m_context, m_port);
  m_acceptor->registerAccept();

  m_contextThread = std::thread([this]() { m_context.run(); });
}

void TcpServerFixture::TearDown()
{
  m_context.stop();
  m_contextThread.join();

  this->TcpFixture::TearDown();
}

auto TcpServerFixture::getTestSockets() -> ConnectedSockets
{
  auto client = this->connectToRunningServer();
  auto server = m_acceptor->waitForServerSocket();

  return ConnectedSockets{.client = std::move(client), .server = std::move(server)};
}

} // namespace test
